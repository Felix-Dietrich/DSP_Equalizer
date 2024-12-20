#this python file serves as the gui for the DSP c program

import ctypes
from pydub import AudioSegment
from pydub.playback import play
import numpy as np
import threading
import pyaudio
import matplotlib.pyplot as plt
import tkinter as tk


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




SAMPLES_PER_BUFFER = 4000
   



hello_world()
filepath = "./example.wav"
audiodata, size, samplerate = load_audio_to_ctypes(filepath)
lib.update_spectrum()
# Audio stream initialisieren (pyaudio)
p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16,
                channels=1,
                rate=samplerate,
                output=True,
                frames_per_buffer=SAMPLES_PER_BUFFER)  # Puffergröße auf die Größe eines Slices setzen




window = tk.Tk()
window.title("Simple Tkinter GUI")

# Add a label with a title
title_label = tk.Label(text="Welcome to the Simple GUI!")
title_label.pack()

slider = tk.Scale(from_=100, to=0, orient='vertical')
slider.pack()
window.update()
# Run the GUI event loop
window.mainloop()




for i in range(0,size-SAMPLES_PER_BUFFER,SAMPLES_PER_BUFFER):
    slice_data = audiodata[i:i+SAMPLES_PER_BUFFER]
    SliceArrayType = ctypes.c_int16 * len(slice_data)
    slice_array = SliceArrayType(*slice_data)
    lib.filterfunction(slice_array,SAMPLES_PER_BUFFER)

     # Umwandeln der verarbeiteten Daten in ein AudioSegment
    # Pydub erwartet die Audiodaten als Bytes, daher müssen wir sie in ein passendes Format bringen
    np_data = np.array(slice_array, dtype=np.int16)
    audio_segment = np_data.tobytes()
    # Audio-Daten streamen
    stream.write(audio_segment)
    #if i == 40*2000:
     #   plt.plot(np_data)
      #  plt.show()

    

