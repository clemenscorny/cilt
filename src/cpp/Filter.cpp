#include "Filter.hpp"

#include <vector>

#ifdef WITH_OPENMP
    #include <omp.h>
#endif // WITH_OPENMP

#include"cilt.h"
#include "Excep.hpp"

namespace cilt {


Filter::Filter()
    : order_(0) {
}


Filter::Filter(std::size_t order)
    : order_(order),
      b_(std::vector<float>(order+1)) {
}


Filter::~Filter() {
}

size_t Filter::getOrder() const {
    return order_;
}

const std::vector<float>* Filter::getNumerator() const {
    return &b_;
}

void Filter::setNumerator(const std::vector<float>& b) {
    if((b.size()-1) != order_) {
        throw Excep("Input array has a wrong size!", CILT_E_WRONG_SIZE);
    }

    b_ = b;
}

FilterTransversal::FilterTransversal()
    : Filter() {
}

FilterTransversal::FilterTransversal(std::size_t order)
    : Filter(order),
      x_(ShiftReg<float>(order+1)) {
}

FilterTransversal::~FilterTransversal() {
}

void FilterTransversal::setCoeffs(const std::vector<float>& b) {
    if((b.size()-1) != order_) {
        resize(b.size()-1);
    }
    setNumerator(b);
}

void FilterTransversal::resize(size_t order) {
    order_ = order;
    x_.resize(order_+1);
}

float FilterTransversal::tick(float data) {
    x_.add(data);
    float result = 0;

#ifdef WITH_OPENMP
    #pragma omp parallel for reduction(+:result)
#endif // WITH_OPENMP
    for(std::size_t i = 0; i <= this->order_; ++i) {
        result += b_[i]*x_[i];
    }

    return result;
}

FilterIIR::FilterIIR()
    : Filter() {
}


FilterIIR::FilterIIR(std::size_t order)
    : Filter(order),
      a_(std::vector<float>(order+1)) {
}


FilterIIR::~FilterIIR() {
}

const std::vector<float>* FilterIIR::getDenumerator() const {
    return &a_;
}

void FilterIIR::setDenumerator(const std::vector<float>& a) {
    if((a.size()-1) != order_) {
        throw Excep("Input array has a wrong size!", CILT_E_WRONG_SIZE);
    }

    a_ = a;

    if(a[0] != 1.) {
        if(a[0] == 0.) {
            throw Excep("First coefficient of denumerator polynom mustn't be 0!",
                    CILT_E_A_0);
        }

        float a0 = a[0];
        std::transform(a_.begin(), a_.end(), a_.begin(),
        std::bind2nd(std::divides<float>(), a0));
        std::transform(b_.begin(), b_.end(), b_.begin(),
        std::bind2nd(std::divides<float>(), a0));
    }
}

void FilterIIR::setCoeffs(const std::vector<float>& a, const std::vector<float>& b) {
    if(a.size() != b.size()) {
        throw Excep("Input arrays have different sizes!", CILT_E_DIFF_SIZES);
    } else if((a.size()-1) != order_) {
        resize(a.size()-1);
    }

    setNumerator(b);
    setDenumerator(a);
}


FilterForm1::FilterForm1()
    : FilterIIR() {
}


FilterForm1::FilterForm1(std::size_t order):
        FilterIIR(order),
        x_(ShiftReg<float>(order+1)),
        y_(ShiftReg<float>(order+1)) {
}


FilterForm1::~FilterForm1() {
}

void FilterForm1::resize(size_t order) {
    order_ = order;
    x_.resize(order_+1);
    y_.resize(order_+1);
}

float FilterForm1::tick(float data) {
    x_.add(data);
    float res = 0;

    for(std::size_t i = 0; i <= this->order_; ++i) {
        res += b_[i]*x_[i];
        if(i != 0) {
            res -= a_[i]*y_[i-1];
        }
    }

    y_.add(res);

    return res;
}

FilterForm2::FilterForm2()
    : FilterIIR() {
}


FilterForm2::FilterForm2(std::size_t order):
        FilterIIR(order),
        u_(ShiftReg<float>(order+1)) {
}


FilterForm2::~FilterForm2() {
}

void FilterForm2::resize(size_t order) {
    order_ = order;
    u_.resize(order_+1);
}

float FilterForm2::tick(float data) {
    float res_a = data;
    float res_b = 0;

    for(std::size_t i = 1; i <= this->order_; ++i) {
        res_a -= a_[i]*u_[i-1];
        res_b += b_[i]*u_[i-1];
    }

    u_.add(res_a);

    return res_a*b_[0]+res_b;
}

}
