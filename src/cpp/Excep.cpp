
#include "Excep.hpp"

#include <exception>
#include <string>

namespace cilt {

Excep::Excep() throw()
    : error_(CILT_E) {
}

Excep::Excep(const Excep& other) throw()
    : std::exception(other),
      what_(other.what_),
      error_(other.error_) {
}

Excep::Excep(const char* msg, int error) throw()
    : what_(msg),
      error_(error) {
}

Excep::Excep(const std::string& msg, int error) throw()
    : what_(msg),
      error_(error) {
}

Excep::~Excep() throw() {
}

const Excep& Excep::operator=(const Excep& other) throw() {
    if(this != &other) {
        what_ = other.what_;
        error_ = other.error_;
    }

    return *this;
}

const char* Excep::what() const throw() {
    return what_.c_str();
}

int Excep::errno() const throw() {
    return error_;
}

}
