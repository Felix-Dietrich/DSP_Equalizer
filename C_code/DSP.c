#include <stdio.h>
#include "filter_convolution.h"
#include "filter_dwt.h"
#include <string.h>


void hello_world() {
    printf("Hello, World!\n");
}


void update_spectrum(float spectrum[SPECTRUMSIZE])
{
    spectrum_to_filter(spectrum, 44100);
}


void filterfunction(uint16_t audio_data[], uint16_t size)
{
    buffer_pcm_t data_in;
    data_in.samplerate = 44100;
    data_in.size = size;
    memcpy(data_in.data, audio_data, size * sizeof(uint16_t));
    buffer_pcm_t data_out; 
    //equalizer(&data_in,&data_out,1);
    filter_dwt(&data_in,&data_out,1);
    memcpy(audio_data, data_out.data, size * sizeof(uint16_t));
}