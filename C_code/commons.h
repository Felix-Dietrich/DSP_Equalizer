#ifndef COMMONS_H
#define COMMONS_H
#include <inttypes.h>

#define SPECTRUMSIZE 20

typedef struct  
{
    uint16_t size;
    uint16_t samplerate;
    int16_t data[4000];
}buffer_pcm_t;


#endif