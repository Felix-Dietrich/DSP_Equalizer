#include "filter_convolution.h"
#include <inttypes.h>
#include "bool.h"
#include <math.h>
#include <stddef.h>


static int16_t filter[FILTERSIZE];

static void equalizer(buffer_pcm_t* in, buffer_pcm_t* out, float volume)
{
    static int16_t in_last[FILTERSIZE];
    int32_t processedData;
   
    out->samplerate = in->samplerate;
    out->size = in->size;

   
    for(int i = 0; i < in->size; i++)  //Faltung
    {
        processedData = 0;
        for (size_t y = 0; y < FILTERSIZE; y++)
        {
            if(i+y<FILTERSIZE)
            {
                processedData += in_last[i+y]*filter[y];
            }
            else
            {
                processedData += in->data[i+y-FILTERSIZE]*filter[y];
            }
            
        }
        out->data[i] = (processedData>>16)*volume; 
    }

    //copy last (Filtersize) samples
    for(int i = 0; i< FILTERSIZE; i++)
    {
        in_last[i] = in->data[in->size-FILTERSIZE+i];
    }

}


static void spectrum_to_filter(const float spectrum[SPECTRUMSIZE], int16_t samplerate)
{
    //clculate factor from dB
    const float start_freq_HZ = 25;
    float spectrum_factor[SPECTRUMSIZE];
    float filter_f[FILTERSIZE/2]={0};
    for(int i = 0; i < SPECTRUMSIZE; i++)
    {
        spectrum_factor[i] = powf(10,spectrum[i]/10); //calculating factor out of dB
    }

    
    float spectrum_factor_all[FILTERSIZE/2]; //first element lowest frequency

    int lowest_frequency = samplerate / FILTERSIZE;
    
    for(int i = 0; i < FILTERSIZE/2; i++) //mapping logarithmicaly spaced spectrum to linearly spaced spectrum
    {
        uint16_t frequency = i*lowest_frequency;
        uint8_t index = (uint8_t)(2*log(frequency/start_freq_HZ)/log(2));
        if(index >= 0 && index < SPECTRUMSIZE)
        {
             spectrum_factor_all[i] = spectrum_factor[index];
        }
    }

    
    spectrum_factor_all[0] /= 2;
    spectrum_factor_all[(FILTERSIZE/2)-1] /= 2;      //weiss ich auch nicht wieso, aber so stimmt das Resultat mit numpy.fft.irfft überein
  

    //discrete fourrier transform
    for(int i = 0; i<FILTERSIZE/2; i++)
    {
        for(int y = 0; y<FILTERSIZE/2; y++)
        {
            filter_f[i] += 1.0/(FILTERSIZE/2-1)*cosf(M_PI/(FILTERSIZE/2-1)*i*y)*spectrum_factor_all[y];
        }
    }

    //kopieren fftshift und in fixed point wandeln
    for(int i= 0; i < FILTERSIZE/2; i++)
    {
        filter[FILTERSIZE/2+i-1] = (1<<13)*filter_f[i];
        filter[i] = (1<<13)*filter_f[(FILTERSIZE/2)-1-i];
    }
}

