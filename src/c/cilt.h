
#ifndef CILT_H
#define CILT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>

enum cilt_Errno {
    CILT_E_NONE,
    CILT_E,
    CILT_E_IND_OUT_OF_RNG,
    CILT_E_WRONG_SIZE,
    CILT_E_DIFF_SIZES,
    CILT_E_A_0
};

typedef void* cilt_Filt;
typedef void* cilt_FiltIIR;
typedef void* cilt_FiltTrv;
typedef void* cilt_FiltForm1;
typedef void* cilt_FiltForm2;

void cilt_filt_del(cilt_Filt filter);
void cilt_filt_getNumerator(cilt_Filt filter, float* b);
int cilt_filt_setNumerator(cilt_Filt filter, size_t order, float* b);
float cilt_filt_tick(cilt_Filt filter, float data);

cilt_FiltTrv cilt_filtTrv_new();
void cilt_filtTrv_setCoeffs(cilt_FiltTrv filter, size_t order, float* b);

void cilt_filtIIR_getDenumerator(cilt_FiltIIR filter, float* a);
int cilt_filtIIR_setDenumerator(cilt_FiltIIR filter, size_t order, float* a);
int cilt_filtIIR_setCoeffs(cilt_FiltIIR filter, size_t order, float* a, float* b);

cilt_FiltForm1 cilt_filtForm1_new();
float cilt_filtForm1_tick(cilt_FiltForm1 filter, float data);

cilt_FiltForm2 cilt_filtForm2_new();
float cilt_filtForm2_tick(cilt_FiltForm2 filter, float data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CILT_H
