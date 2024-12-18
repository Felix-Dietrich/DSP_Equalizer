#this python file serves as the gui for the DSP c program

import ctypes

# Bibliothek laden
lib = ctypes.CDLL("./C_code/library.so")

lib.filterfunction.argtypes = [ctypes.POINTER(ctypes.c_int16), ctypes.c_size_t]
lib.filterfunction.restype = None


# Funktion aufrufen
def process_audio(file, option):
    ArrayType = ctypes.c_int16 * 2000
    audio = ArrayType()
    lib.filterfunction(audio,len(audio))

def  hello_world():
    lib.hello_world()

hello_world()