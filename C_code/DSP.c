#include <stdio.h>
#include "filter_convolution.c"


void hello_world() {
    printf("Hello, World!\n");
}

const float spectrum[SPECTRUMSIZE] = {12,12,6,6,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void update_spectrum()
{
    spectrum_to_filter(spectrum, (int16_t)44100);
}


void filterfunction(uint16_t audio_data[], uint16_t size)
{
    static buffer_pcm_t data_in;
    data_in.samplerate = 44100;
    data_in.size = size;
    for(int i = 0; i< size;i++)
    {
        data_in.data[i] = audio_data[i];
    }
    static buffer_pcm_t data_out;

    equalizer(&data_in,&data_out,0.5);

    audio_data = data_out.data;
}