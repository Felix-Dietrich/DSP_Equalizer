#include "commons.h"

//assuming input buffer is at least big enugh to do one full conversion (2^stages)



#define WAVELET_SIZE 2
#define FILTERLENGTH 1024
#define DWT_LEVELS  10

const float D2[] = {1,1};
const float D4[] = {0.6830127,1.1830127,0.3169873,-0.1830127};
const float D6[] = {0.47046721,1.14111692,0.650365,-0.19093442,-0.12083221,0.0498175};


const float* wavelet = D2;

typedef int16_t dwt_coefficients_t[2*DWT_LEVELS][FILTERLENGTH/2];




static void filter_dwt(buffer_pcm_t* in, buffer_pcm_t* out, float volume)
{
    dwt_coefficients_t dwt_coefficients;
    decomposition(in, &dwt_coefficients);
}

static void decomposition(buffer_pcm_t* in, dwt_coefficients_t* coefficients)
{
    int size = FILTERLENGTH;
    int16_t detail[FILTERLENGTH>>1];
    int16_t* temp_p;
    int16_t next_in[FILTERLENGTH>>1];
    int16_t approximation[FILTERLENGTH>>1];
    int16_t* next_in_p = next_in;
    int16_t* approximation_p = approximation;

    apply_wavelets(in, detail, approximation, size);
    size/=2;
    for(int i = 0; i < 8;i++)
    {
        apply_wavelets(detail, coefficients[i],coefficients[i+1],size);
        apply_wavelets(next_in_p, detail, approximation_p, size);
        //swap pointers
        temp_p = next_in_p;
        next_in_p = approximation_p;
        approximation_p = temp_p;
        size/=2;
    }
    apply_wavelets(detail, coefficients[16],coefficients[17],size);
    apply_wavelets(next_in_p, coefficients[18], coefficients[19], size);
}

static void reconstruction()
{

}

static void apply_wavelets(int16_t in[], int16_t detail[], int16_t approximation[],int size)
{
    for(int i = 0; i < size; i+=2)
    {
        apply_wavelet(&in[i],&detail[i/2],&approximation[i/2]);
    }

}

static void apply_wavelet(int16_t in[WAVELET_SIZE], int16_t* detail, int16_t* approximation)
{
    *approximation = 0;
    *detail = 0;
    for(int i = 0; i < (WAVELET_SIZE-1); i+=2)
    {
        *approximation += (in[i] * wavelet[i])/2;
        *approximation += (in[i+1] * wavelet[i+1])/2;
        *detail += (in[i]* wavelet[WAVELET_SIZE-1-i]);
        *detail -= (in[i+1]* wavelet[WAVELET_SIZE-2-i]);
    }
  
}