
#ifndef CILT_H
#define CILT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void* cilt_Filt;
typedef void* cilt_FittIIR;
typedef void* cilt_FiltTrv;
typedef void* cilt_FiltForm1;
typedef void* cilt_FiltForm2;

void cilt_filt_del(cilt_Filt filter);
void cilt_filt_getNumerator(cilt_Filt filter, float* b);
void cilt_filt_setNumerator(cilt_Filt filter, size_t order, float* b);

cilt_FiltTrv cilt_filtTrv_new();
float cilt_filtTrv_tick(cilt_FiltTrv filter, float data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CILT_H
