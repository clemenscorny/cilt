#include "cilt.hpp"
    using cilt::Filter;
    using cilt::FilterTransversal;
#include "cilt.h"

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

void cilt_filt_setNumerator(cilt_Filt filter, size_t order, float* b) {
    Filter* filter_cpp = static_cast<Filter*>(filter);
    vector<float> b_vec(b, b+order);

    filter_cpp->setNumerator(b_vec);
}

cilt_FiltTrv cilt_filtTrv_new() {
    FilterTransversal* filter_cpp = new FilterTransversal();
    return filter_cpp;
}

void cilt_filtTrv_setNumerator(cilt_FiltTrv filter, float* b) {
    FilterTransversal* filter_cpp = static_cast<FilterTransversal*>(filter);
    vector<float> b_vec(b, b+filter_cpp->getOrder());
    filter_cpp->setNumerator(b_vec);
}

float cilt_filtTrv_tick(cilt_FiltTrv filter, float data) {
    FilterTransversal* filter_cpp = static_cast<FilterTransversal*>(filter);
    return filter_cpp->tick(data);
}
