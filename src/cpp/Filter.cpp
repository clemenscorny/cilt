#include "Filter.hpp"

#include <vector>

namespace cilt {


Filter::Filter()
    : order_(0) {
}


Filter::Filter(std::size_t order)
    : order_(order),
      b_(std::vector<float>(order)) {
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
    if(b.size() != order_) {
        resize(b.size());
    }

    b_ = b;
}

FilterTransversal::FilterTransversal()
    : Filter() {
}

FilterTransversal::FilterTransversal(std::size_t order)
    : Filter(order),
      x_(ShiftReg<float>(order)) {
}

FilterTransversal::~FilterTransversal() {
}

void FilterTransversal::resize(size_t size) {
    order_ = size;
    x_.resize(order_);
}

float FilterTransversal::tick(float data) {
    x_.add(data);
    float result = 0;

    for(std::size_t i = 0; i < this->order_; ++i) {
        result += b_[i]*x_[i];
    }

    return result;
}

FilterIIR::FilterIIR()
    : Filter() {
}


FilterIIR::FilterIIR(std::size_t order)
    : Filter(order),
      a_(std::vector<float>(order)) {
}


FilterIIR::~FilterIIR() {
}

const std::vector<float>* FilterIIR::getDenumerator() const {
    return &a_;
}

void FilterIIR::setDenumerator(const std::vector<float>& a) {
    if(a.size() != order_) {
        return;
    }

    a_ = a;

    if(a[0] != 1) {
        float a0 = a[0];
        std::transform(a_.begin(), a_.end(), a_.begin(),
        std::bind2nd(std::divides<float>(), a0));
        std::transform(b_.begin(), b_.end(), b_.begin(),
        std::bind2nd(std::divides<float>(), a0));
    }
}

void FilterIIR::init(const std::vector<float>& a, const std::vector<float>& b) {
    if(a.size() != b.size()) {
        return;
    } else if(a.size() != order_) {
        resize(a.size());
    }
    setNumerator(b);
    setDenumerator(a);
}


FilterForm1::FilterForm1()
    : FilterIIR() {
}


FilterForm1::FilterForm1(std::size_t order):
        FilterIIR(order),
        x_(ShiftReg<float>(order)),
        y_(ShiftReg<float>(order)) {
}


FilterForm1::~FilterForm1() {
}

void FilterForm1::resize(size_t size) {
    order_ = size;
    x_.resize(order_);
    y_.resize(order_);
}

float FilterForm1::tick(float data) {
    x_.add(data);
    float res = 0;

    for(std::size_t i = 0; i < this->order_; ++i) {
        res += b_[i]*x_[i];
        if(i != 0) {
            res -= a_[i]*y_[i];
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
        u_(ShiftReg<float>(order)) {
}


FilterForm2::~FilterForm2() {
}

void FilterForm2::resize(size_t size) {
    order_ = size;
    u_.resize(order_);
}

float FilterForm2::tick(float data) {
    float res_a = data;
    float res_b = 0;

    for(std::size_t i = 1; i < this->order_; ++i) {
        res_a -= a_[i]*u_[i];
        res_b += b_[i]*u_[i];
    }

    u_.add(res_a);

    return res_a*b_[0]+res_b;
}

}
