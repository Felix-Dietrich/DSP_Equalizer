#this python file serves as the gui for the DSP c program

import ctypes

# Bibliothek laden
lib = ctypes.CDLL("./C_code/library.so")

# Funktion aufrufen
def process_audio(file, option):
    lib.process_audio(ctypes.c_char_p(file.encode()), ctypes.c_char_p(option.encode()))

def  hello_world():
    lib.hello_world()

hello_world()