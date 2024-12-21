#this python file serves as the gui for the DSP c program

import ctypes
from pydub import AudioSegment
from pydub.playback import play
import numpy as np
import threading
import pyaudio
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import ttk, filedialog
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg


# Bibliothek laden
lib = ctypes.CDLL("./C_code/library.so")

lib.filterfunction.argtypes = [ctypes.POINTER(ctypes.c_int16), ctypes.c_size_t]
lib.filterfunction.restype = None


def  hello_world():
    lib.hello_world()


def update_spectrum():
    lib.update_spectrum()

def open_file():
    file_path = filedialog.askopenfilename(filetypes=[("Audio Files", "*.wav *.mp3")])
    if file_path:
        file_label.config(text=f"Selected file: {file_path}")

def update_plot():
    spectrum = [(0-slider.get()) for slider in sliders]
    ax.clear()
    ax.bar(range(1, 21), spectrum, color='blue')
    ax.set_ylim(-20, 20)
    canvas.draw()
    

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

def audio_processing():
    SAMPLES_PER_BUFFER = 4000
    hello_world()
    filepath = "./example.wav"
    audiodata, size, samplerate = load_audio_to_ctypes(filepath)
    update_spectrum()
    # Audio stream initialisieren (pyaudio)
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16,
                    channels=1,
                    rate=samplerate,
                    output=True,
                    frames_per_buffer=SAMPLES_PER_BUFFER)  # Puffergröße auf die Größe eines Slices setzen


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





# Main window
root = tk.Tk()
root.title("Equalizer GUI")

# File selection
file_frame = ttk.Frame(root)
file_frame.pack(pady=10)
file_button = ttk.Button(file_frame, text="Choose File", command=open_file)
file_button.pack(side=tk.LEFT, padx=5)
file_label = ttk.Label(file_frame, text="No file selected")
file_label.pack(side=tk.LEFT)

# Equalizer sliders
slider_frame = ttk.Frame(root)
slider_frame.pack(pady=20)
label = ttk.Label(slider_frame, text="+20dB\n\n\n0dB\n\n\n-20dB")
label.grid(row=0, column=0, padx=2) 

label = ttk.Label(slider_frame, text="Freq(Hz)")
label.grid(row=1, column=0, padx=2) 
sliders = []
for i in range(20):
    slider = ttk.Scale(slider_frame, from_=-20, to=20, orient='vertical', command=lambda e: update_plot())
    slider.grid(row=0, column=i+1, padx=2)  # Place slider in a grid
   # Create the label below the slider
    label = ttk.Label(slider_frame, text=f"{25*(2**(i/2)):.0f}")
    label.grid(row=1, column=i+1, padx=2) 
    sliders.append(slider)


# Plot area
fig, ax = plt.subplots(figsize=(5, 3))
ax.set_title("Equalizer Spectrum")
ax.set_xlabel("Frequency Band")
ax.set_ylabel("Gain")
canvas = FigureCanvasTkAgg(fig, master=root)
canvas_widget = canvas.get_tk_widget()
canvas_widget.pack()


# Start the background task in a separate thread
thread = threading.Thread(target=audio_processing, daemon=True)
thread.start()

# Run the GUI
root.mainloop()



    

