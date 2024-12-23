#include "commons.h"

//assuming input buffer is at least big enugh to do one full conversion (2^stages)



#define WAVELET_SIZE 2
#define FILTERLENGTH 1024

const float D2[] = {1,1};
const float D4[] = {0.6830127,1.1830127,0.3169873,-0.1830127};
const float D6[] = {0.47046721,1.14111692,0.650365,-0.19093442,-0.12083221,0.0498175};


const float* wavelet = D2;

typedef struct 
{
    int16_t HH[FILTERLENGTH>>2];
    int16_t HL[FILTERLENGTH>>2];
    int16_t LHH[FILTERLENGTH>>3];
    int16_t LHL[FILTERLENGTH>>3];
    int16_t LLHH[FILTERLENGTH>>4];
    int16_t LLHL[FILTERLENGTH>>4];
    int16_t LLLHH[FILTERLENGTH>>5];
    int16_t LLLHL[FILTERLENGTH>>5];
    int16_t LLLLHH[FILTERLENGTH>>6];
    int16_t LLLLHL[FILTERLENGTH>>6];
    int16_t LLLLLHH[FILTERLENGTH>>7];
    int16_t LLLLLHL[FILTERLENGTH>>7];
    int16_t LLLLLLHH[FILTERLENGTH>>8];
    int16_t LLLLLLHL[FILTERLENGTH>>8];
    int16_t LLLLLLLHH[FILTERLENGTH>>9];
    int16_t LLLLLLLHL[FILTERLENGTH>>9];
    int16_t LLLLLLLLHH[FILTERLENGTH>>10];
    int16_t LLLLLLLLHL[FILTERLENGTH>>10];
    int16_t LLLLLLLLLH[FILTERLENGTH>>10];
    int16_t LLLLLLLLLL[FILTERLENGTH>>10];
}dwt_coefficients_t;




static void filter_dwt(buffer_pcm_t* in, buffer_pcm_t* out, float volume)
{
    
}

static void decomposition(buffer_pcm_t* in, dwt_coefficients_t* coefficients)
{
    int size = FILTERLENGTH;
    int16_t detail[FILTERLENGTH>>1];
    int16_t approximation[FILTERLENGTH>>1];
    int16_t approximation2[FILTERLENGTH>>1];
    apply_wavelets(in, detail, approximation, size);
    size/=2;
    apply_wavelets(detail, coefficients->HH,coefficients->HL,size);
    apply_wavelets(approximation, detail, approximation2, size);
    size/=2;
    apply_wavelets(detail, coefficients->LHH,coefficients->LHL,size);
    apply_wavelets(approximation2, detail, approximation, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLHH,coefficients->LLHL,size);
    apply_wavelets(approximation, detail, approximation2, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLLHH,coefficients->LLLHL,size);
    apply_wavelets(approximation2, detail, approximation, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLLLHH,coefficients->LLLLHL,size);
    apply_wavelets(approximation, detail, approximation2, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLLLLHH,coefficients->LLLLLHL,size);
    apply_wavelets(approximation2, detail, approximation, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLLLLLHH,coefficients->LLLLLLHL,size);
    apply_wavelets(approximation, detail, approximation2, size);
    size/=2;
    apply_wavelets(detail, coefficients->LLLLLLLHH,coefficients->LLLLLLLHL,size);
    apply_wavelets(approximation2, detail, approximation, size);
    size/2;
    apply_wavelets(detail, coefficients->LLLLLLLLHH,coefficients->LLLLLLLLHL,size);
    apply_wavelets(approximation, coefficients->LLLLLLLLLH, coefficients->LLLLLLLLLL, size);
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