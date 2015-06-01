#ifndef SHIFTREG_HPP
#define SHIFTREG_HPP

#include <algorithm>
#include <iterator>

#include "Excep.hpp"
#include "cilt.h"

namespace cilt {

/**
 * Shift register shaped like a ring buffer
 */
template <class T>
class ShiftReg {
    std::size_t n_; /**< size of the buffer */
    std::size_t pos_; /**< current head of the buffer */
    T *data_; /**< array containing the data */

  public:
    ShiftReg();
    /**
     * Create buffer with a defined size
     * @param size buffer size
     */
    explicit ShiftReg(std::size_t size);
    /**
     * Copy constructor
     */
    ShiftReg(const ShiftReg& other);
    virtual ~ShiftReg();

    /**
     * Copy all values into lhs object of the assignment
     */
    const ShiftReg<T>& operator=(const ShiftReg<T>& other);
    /**
     * Index the buffer like an array
     * @param ind index
     */
    T operator[](std::size_t ind) const;

    /**
     * Get size of buffer
     * @return size
     */
    std::size_t size() const;
    /**
     * Shift register one step to the right side (last element pops) and put
     *     given element to the head
     * @param element new element
     */
    void add(const T &element);
    /**
    * Get element of buffer
    * @param ind index
    */
    T at(std::size_t ind) const;
    /**
     * Set all values of the register
     * @param val value
     */
    void set(T val);

    void resize(size_t size);
};

template <class T>
ShiftReg<T>::ShiftReg()
    : n_(0),
      pos_(0),
      data_(NULL) {
}

template <class T>
ShiftReg<T>::ShiftReg(std::size_t size)
    : n_(size),
      pos_(0),
      data_(new T[n_]) {
    set(0);
}

template <class T>
ShiftReg<T>::ShiftReg(const ShiftReg& other)
    : n_(0),
      pos_(0),
      data_(NULL) {
    if(other.data_ != NULL) {
        data_ = new T[other.n_];
        std::copy(other.data_, other.data_+other.n_, data_);
        n_ = other.n_;
        pos_ = other.pos_;
    }
}


template <class T>
ShiftReg<T>::~ShiftReg() {
    if(data_ != NULL) {
        delete[] data_;
        data_ = NULL;
    }
}

template <class T>
T ShiftReg<T>::at(std::size_t ind) const {
    if(ind >= n_) {
        throw Excep("Index out of range!", CILT_IND_OUT_OF_RNG);
    }

    return data_[(pos_+ind)%n_];
}

template <class T>
const ShiftReg<T>& ShiftReg<T>::operator=(const ShiftReg<T>& other) {
    if(this == &other) {
        return *this;
    }

    if(data_ != NULL) {
        delete[] data_;
        data_ = NULL;
    }
    n_ = other.n_;
    pos_ = other.pos_;
    if(n_ != 0) {
        data_ = new T[n_];
        std::copy(other.data_, other.data_+other.n_, data_);
    }

    return *this;
}

template <class T>
T ShiftReg<T>::operator[](std::size_t ind) const {
    return at(ind);
}

template <class T>
std::size_t ShiftReg<T>::size() const {
    return n_;
}

template <class T>
void ShiftReg<T>::add(const T &element) {
    if(n_ == 0) {
        return;
    }

    // virtual shift (decrement head about one)
    if(pos_ != 0) {
        --pos_;
    // index jumps from the first element of the array to the last
    } else {
        pos_ = n_-1;
    }

    data_[pos_] = element;
}

template <class T>
void ShiftReg<T>::set(T val) {
    std::fill(data_, data_+n_, val);
}

template <class T>
void ShiftReg<T>::resize(size_t size) {
    if(data_ != NULL) {
        delete[] data_;
        data_ = NULL;
    }

    n_ = size;
    pos_ = 0;
    data_ = new T[n_];
    set(0);
}

}

#endif // SHIFTREG_HPP
