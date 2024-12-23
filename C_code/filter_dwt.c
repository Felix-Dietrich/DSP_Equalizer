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
    reconstruction(out,&dwt_coefficients);
}

static void decomposition(buffer_pcm_t* in, dwt_coefficients_t* coefficients)
{
    int size = FILTERLENGTH;
    int16_t detail[FILTERLENGTH/2];
    int16_t* temp_p;
    int16_t next_in[FILTERLENGTH/2];
    int16_t approx[FILTERLENGTH/2];
    int16_t* next_in_p = next_in;
    int16_t* approx_p = approx;

    decompose_wavelets(in->data, detail, approx, size);
    size/=2;
    for(int i = 0; i < 16;i+=2)
    {
        decompose_wavelets(detail, coefficients[i],coefficients[i+1],size);
        decompose_wavelets(next_in_p, detail, approx_p, size);
        //swap pointers
        temp_p = next_in_p;
        next_in_p = approx_p;
        approx_p = temp_p;
        size/=2;
    }
    decompose_wavelets(detail, coefficients[16],coefficients[17],size);
    decompose_wavelets(next_in_p, coefficients[18], coefficients[19], size);
}




static void reconstruction(buffer_pcm_t* out, dwt_coefficients_t* coefficients)
{
    int16_t detail[FILTERLENGTH/2];
    int16_t approx[FILTERLENGTH/2];
    int16_t next_out[FILTERLENGTH/2];
    int16_t size = 2;
    int16_t* next_out_p = next_out;
    int16_t* approx_p = approx;
    int16_t* temp_p;

    reconstruct_wavelets(next_out_p,coefficients[18],coefficients[19],size);
    reconstruct_wavelets(detail,coefficients[16],coefficients[17],size);

    for(int i = 15; i>=1;i-=2)
    {
        temp_p = next_out_p;
        next_out_p = approx_p;
        approx_p = temp_p;
        size*=2;
        reconstruct_wavelets(next_out_p,detail,approx_p,size);
        reconstruct_wavelets(detail,coefficients[i-1],coefficients[i],size);
    }
    size*=2;
    reconstruct_wavelets(out->data,detail,next_out_p,size);
    out->size=size;
}

static void decompose_wavelets(int16_t in[], int16_t detail[], int16_t approx[],int size)
{
    for(int i = 0; i < size/2; i++)
    {
        decompose_wavelet(&in[i*2],&detail[i],&approx[i]);
    }

}


/**
 *  Beislpiel mit haar wavelet
 *   in  1    2
 *   detail  0.5
 *   approx  1.5
 */
static void decompose_wavelet(int16_t in[WAVELET_SIZE], int16_t* detail, int16_t* approx)
{
    *approx = 0;
    *detail = 0;
    for(int i = 0; i < (WAVELET_SIZE-1); i+=2)
    {
        *approx += (in[i] * wavelet[i])/2;
        *approx += (in[i+1] * wavelet[i+1])/2;
        *detail += (in[i]* wavelet[WAVELET_SIZE-1-i])/2;
        *detail -= (in[i+1]* wavelet[WAVELET_SIZE-2-i])/2;
    }
}


static void reconstruct_wavelets(int16_t out[], int16_t detail[], int16_t approx[],int size)
{
    for(int i = 0; i < size/2; i++)
    {
        reconstruct_wavelet(out[i*2],&detail[i],&approx[i]);
    }
}

/**
 *  Beislpiel mit haar wavelet
 *   detail  0.5 
 *   approx  1.5
 *   out  1    2 
 */
static void reconstruct_wavelet(int16_t out[WAVELET_SIZE], int16_t* detail, int16_t* approx)
{
    for(int i = 0; i < (WAVELET_SIZE-1); i+=2)
    {
        out[i] += (*approx * wavelet[WAVELET_SIZE-1-i]);
        out[i+1] += (*approx * wavelet[WAVELET_SIZE-2-i]);
        out[i] -= (*detail * wavelet[i])/2;
        out[i+1] += (*detail * wavelet[i+1]);
    }
}


