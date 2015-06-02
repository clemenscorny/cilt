#include "cilt.h"

#include "cilt.hpp"
    using cilt::Filter;
    using cilt::FilterTransversal;
    using cilt::FilterIIR;
    using cilt::FilterForm1;
    using cilt::FilterForm2;
#include "Excep.hpp"
    using cilt::Excep;

#include <vector>
    using std::vector;

void cilt_filt_del(cilt_Filt filter) {
    Filter* filter_cpp = static_cast<Filter*>(filter);
    delete filter_cpp;
}

void cilt_filt_getNumerator(cilt_Filt filter, float* b) {
    Filter* filter_cpp = static_cast<Filter*>(filter);
    const vector<float>* b_vec = filter_cpp->getNumerator();
    for(size_t i = 0; i < filter_cpp->getOrder(); ++i) {
        b[i] = b_vec->at(i);
    }
}

int cilt_filt_setNumerator(cilt_Filt filter, size_t order, float* b) {
    Filter* filter_cpp = static_cast<Filter*>(filter);
    vector<float> b_vec(b, b+order);
    int errno = CILT_E_NONE;

    try {
        filter_cpp->setNumerator(b_vec);
    } catch(const Excep &e) {
        errno = e.errno();
    }

    return errno;
}

cilt_FiltTrv cilt_filtTrv_new() {
    FilterTransversal* filter_cpp = new FilterTransversal();
    return filter_cpp;
}

void cilt_filtTrv_setCoeffs(cilt_FiltTrv filter, size_t order, float* b) {
    FilterTransversal* filter_cpp = static_cast<FilterTransversal*>(filter);
    vector<float> b_vec(b, b+order);
    filter_cpp->setCoeffs(b_vec);
}


int cilt_filtTrv_setNumerator(cilt_FiltTrv filter, float* b) {
    FilterTransversal* filter_cpp = static_cast<FilterTransversal*>(filter);
    vector<float> b_vec(b, b+filter_cpp->getOrder());
    int errno = CILT_E_NONE;

    try {
        filter_cpp->setNumerator(b_vec);
    } catch(const Excep &e) {
        errno = e.errno();
    }

    return errno;
}

float cilt_filt_tick(cilt_Filt filter, float data) {
    Filter* filter_cpp = static_cast<Filter*>(filter);
    return filter_cpp->tick(data);
}

void cilt_filtIIR_getDenumerator(cilt_FiltIIR filter, float* a) {
    FilterIIR* filter_cpp = static_cast<FilterIIR*>(filter);
    const vector<float>* a_vec = filter_cpp->getDenumerator();
    for(size_t i = 0; i < filter_cpp->getOrder(); ++i) {
        a[i] = a_vec->at(i);
    }
}

int cilt_filtIIR_setDenumerator(cilt_FiltIIR filter, size_t order, float* a) {
    FilterIIR* filter_cpp = static_cast<FilterIIR*>(filter);
    vector<float> a_vec(a, a+order);
    int errno = CILT_E_NONE;

    try {
        filter_cpp->setDenumerator(a_vec);
    } catch(const Excep &e) {
        errno = e.errno();
    }

    return errno;
}

int cilt_filtIIR_setCoeffs(cilt_FiltIIR filter, size_t order, float* a, float* b) {
    FilterIIR* filter_cpp = static_cast<FilterIIR*>(filter);
    vector<float> a_vec(a, a+order);
    vector<float> b_vec(b, b+order);
    int errno = CILT_E_NONE;

    try {
        filter_cpp->setCoeffs(a_vec, b_vec);
    } catch(const Excep &e) {
        errno = e.errno();
    }

    return errno;
}

cilt_FiltForm1 cilt_filtForm1_new() {
    FilterForm1* filter_cpp = new FilterForm1();
    return filter_cpp;
}

float cilt_filtForm1_tick(cilt_FiltForm1 filter, float data) {
    FilterForm1* filter_cpp = static_cast<FilterForm1*>(filter);
    return filter_cpp->tick(data);
}

cilt_FiltForm2 cilt_filtForm2_new() {
    FilterForm2* filter_cpp = new FilterForm2();
    return filter_cpp;

}

float cilt_filtForm2_tick(cilt_FiltForm2 filter, float data) {
    FilterForm2* filter_cpp = static_cast<FilterForm2*>(filter);
    return filter_cpp->tick(data);
}
