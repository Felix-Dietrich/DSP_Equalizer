#this python file serves as the gui for the DSP c program

import ctypes
from pydub import AudioSegment
from pydub.playback import play
import numpy as np
import threading
import pyaudio


# Bibliothek laden
lib = ctypes.CDLL("./C_code/library.so")

lib.filterfunction.argtypes = [ctypes.POINTER(ctypes.c_int16), ctypes.c_size_t]
lib.filterfunction.restype = None

def load_audio_to_ctypes(filepath):
    # Load the audio file using pydub
    audio = AudioSegment.from_file(filepath)

    # Convert to PCM 16-bit mono (if not already)
    audio = audio.set_frame_rate(44100).set_sample_width(2).set_channels(1)

    # Get raw PCM data as bytes
    raw_data = audio.raw_data

    # Convert bytes to a numpy array of int16
    audio_data = np.frombuffer(raw_data, dtype=np.int16)

    # Create ctypes array
    ArrayType = ctypes.c_int16 * len(audio_data)
    ctypes_array = ArrayType(*audio_data)

    return ctypes_array, len(audio_data), audio.frame_rate




   

def  hello_world():
    lib.hello_world()

hello_world()
filepath = "./example.wav"
audiodata, size, samplerate = load_audio_to_ctypes(filepath)
for i in range(0,size-2000,2000):
    slice_data = audiodata[i:i+2000]
    SliceArrayType = ctypes.c_int16 * len(slice_data)
    #lib.filterfunction(SliceArrayType(*slice_data),2000)

     # Umwandeln der verarbeiteten Daten in ein AudioSegment
    # Pydub erwartet die Audiodaten als Bytes, daher müssen wir sie in ein passendes Format bringen
    np_data = np.array(slice_data, dtype=np.int16)
    audio_segment = AudioSegment(
        np_data.tobytes(), 
        frame_rate=samplerate,
        sample_width=2,  # 16-bit PCM
        channels=1  # Mono
    )
    
    # Das Audio-Slice abspielen
    play(audio_segment)

