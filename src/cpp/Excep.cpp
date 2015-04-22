
#include "Excep.hpp"

#include <exception>
#include <string>

namespace cilt {

Excep::Excep() throw() {
}

Excep::Excep(const Excep& other) throw()
    : std::exception(other),
      what_(other.what_) {
}

Excep::Excep(const char* msg) throw()
    : what_(msg) {
}

Excep::Excep(const std::string& msg) throw()
    : what_(msg) {
}

Excep::~Excep() throw() {
}

const Excep& Excep::operator=(const Excep& other) throw() {
    if(this != &other) {
        what_ = other.what_;
    }

    return *this;
}

const char* Excep::what() const throw() {
    return what_.c_str();
}

};
