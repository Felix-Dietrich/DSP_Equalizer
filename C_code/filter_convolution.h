#ifndef FILTER_CONVOLUTION
#define FILTER_CONVOLUTION


#define FILTERSIZE 2048
#include "commons.h"

void spectrum_to_filter(const float spectrum[SPECTRUMSIZE], uint32_t samplerate);
void equalizer(buffer_pcm_t* in, buffer_pcm_t* out, float volume);

#endif