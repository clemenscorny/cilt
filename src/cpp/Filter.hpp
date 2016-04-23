#ifndef FILTER_HPP
#define FILTER_HPP

#include <algorithm>
#include <vector>

#include "ShiftReg.hpp"

namespace cilt {

/**
 * Abstract class to define some basic filtering functions.
 */
class Filter
{
protected:
    std::size_t order_; /**< filter order */
    std::vector<float> b_; /**< numerator coefficients */
public:
    Filter();
    /**
     * Create Filter with defined order
     * @param order filter order
     */
    explicit Filter(std::size_t order);
    virtual ~Filter();

    /**
     * Get order of the filter
     * @return filter order
     */
    virtual size_t getOrder() const;
    /**
     * Set numerator coefficients
     * @param b numerator coefficients
     */
    virtual void setNumerator(const std::vector<float>& b);
    /**
     * Get numerator coefficients
     * @return numerator coefficients
     */
    virtual const std::vector<float>* getNumerator() const;

    /**
     * Change order of the filter
     * @praram filter new filter order
     */
    virtual void resize(size_t order) = 0;
    /**
     * Filter data
     * @param data new data to be processed
     * @return filtering result
     */
    virtual float tick(float data) = 0;
};

/**
 * Transversal filter (finite impulse response)
 */
class FilterTransversal: public Filter {
protected:
    ShiftReg<float> x_; /**< input value memory chain */
public:
    FilterTransversal();
    explicit FilterTransversal(std::size_t order);
    virtual ~FilterTransversal();

    /**
     * Set all coefficients of the filter and resize if necessary
     * @param b numerator coefficients
     */
    virtual void setCoeffs(const std::vector<float>& b);

    virtual void resize(size_t order);
    virtual float tick(float data);
};

/**
 * Abstract class defining some basic functions of infinite impulse response
 * filters.
 */
class FilterIIR: public Filter {
protected:
    std::vector<float> a_; /**< denumerator coefficients */
public:
    FilterIIR();
    explicit FilterIIR(std::size_t order);
    virtual ~FilterIIR();

    /**
     * Get denumerator coefficients
     * @return denumerator coefficients
     */
    virtual const std::vector<float>* getDenumerator() const;
    /**
     * Set denumerator coefficients
     * @param a denumerator coefficients
     */
    virtual void setDenumerator(const std::vector<float>& a);
    /**
     * Set all coefficients of the filter and resize if necessary
     * @param a denumerator coefficients
     * @param b numerator coefficients
     */
    virtual void setCoeffs(const std::vector<float>& a, const std::vector<float>& b);
};

/**
 * Filter form 1 (infinite impulse response)
 * It has 2 memory chains, 1 for the input and 1 for the output values.
 */
class FilterForm1: public FilterIIR {
protected:
    ShiftReg<float> x_; /**< input value memory chain */
    ShiftReg<float> y_; /**< output value memory chain */
public:
    FilterForm1();
    explicit FilterForm1(std::size_t order);
    virtual ~FilterForm1();

    virtual void resize(size_t order);
    virtual float tick(float data);
};

/**
 * Filter form 2 (infinite impulse response)
 * It has 1 combined memory chain for input and output values.
 */
class FilterForm2: public FilterIIR {
protected:
    ShiftReg<float> u_; /**< combined input and output memory chain */
public:
    FilterForm2();
    explicit FilterForm2(std::size_t order);
    virtual ~FilterForm2();

    virtual void resize(size_t order);
    virtual float tick(float data);
};

}

#endif // FILTER_HPP
