#include <stdio.h>
#include "filter_convolution.c"
#include <string.h>


void hello_world() {
    printf("Hello, World!\n");
}

const float spectrum[SPECTRUMSIZE] ={12,12,6,6,3,3,0,0,0,0,0,-12,-12,-12,-12,-12,-12,-12,-12,-12};

void update_spectrum()
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
    equalizer(&data_in,&data_out,2);
    memcpy(audio_data, data_out.data, size * sizeof(uint16_t));
}