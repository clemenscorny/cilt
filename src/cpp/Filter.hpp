#ifndef FILTER_HPP
#define FILTER_HPP

#include <algorithm>
#include <vector>

#include "ShiftReg.hpp"

namespace cilt {

class Filter
{
protected:
    std::size_t order_;
    std::vector<float> b_;
public:
    Filter();
    explicit Filter(std::size_t order);
    virtual ~Filter();

    virtual size_t getOrder() const;
    virtual void setNumerator(const std::vector<float>& b);
    virtual const std::vector<float>* getNumerator() const;

    virtual void resize(size_t size) = 0;
    virtual float tick(float data) = 0;
};

class FilterTransversal: public Filter {
protected:
    ShiftReg<float> x_;
public:
    FilterTransversal();
    explicit FilterTransversal(std::size_t order);
    virtual ~FilterTransversal();

    virtual void setCoeffs(const std::vector<float>& b);

    virtual void resize(size_t size);
    virtual float tick(float data);
};

class FilterIIR: public Filter {
protected:
    std::vector<float> a_;
public:
    FilterIIR();
    explicit FilterIIR(std::size_t order);
    virtual ~FilterIIR();

    virtual const std::vector<float>* getDenumerator() const;
    virtual void setDenumerator(const std::vector<float>& a);
    virtual void setCoeffs(const std::vector<float>& a, const std::vector<float>& b);
};

class FilterForm1: public FilterIIR {
protected:
    ShiftReg<float> x_;
    ShiftReg<float> y_;
public:
    FilterForm1();
    explicit FilterForm1(std::size_t order);
    virtual ~FilterForm1();

    virtual void resize(size_t size);
    virtual float tick(float data);
};

class FilterForm2: public FilterIIR {
protected:
    ShiftReg<float> u_;
public:
    FilterForm2();
    explicit FilterForm2(std::size_t order);
    virtual ~FilterForm2();

    virtual void resize(size_t size);
    virtual float tick(float data);
};

}

#endif // FILTER_HPP
