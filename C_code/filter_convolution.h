#ifndef FILTER_CONVOLUTION
#define FILTER_CONVOLUTION

#define SPECTRUMSIZE 20
#define FILTERSIZE 1024
#include <inttypes.h>

typedef struct  
{
    uint16_t size;
    uint16_t samplerate;
    uint16_t data[2000];
}buffer_pcm_t;

static void spectrum_to_filter(const float spectrum[SPECTRUMSIZE], int16_t samplerate);
static void equalizer(buffer_pcm_t* in, buffer_pcm_t* out, float volume);

#endif