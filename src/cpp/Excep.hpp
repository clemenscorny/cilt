#ifndef EXCEP_HPP
#define EXCEP_HPP

#include <exception>
#include <string>

#include "cilt.h"

namespace cilt {

class Excep: public std::exception {
protected:
    enum cilt_Errno error_;
    std::string what_;
public:
    Excep() throw();
    Excep(const Excep& other) throw();
    Excep(const char* msg, cilt_Errno errno = CILT_E) throw();
    Excep(const std::string& msg, cilt_Errno errno = CILT_E) throw();
    virtual ~Excep() throw();
    const Excep& operator=(const Excep& other) throw();
    virtual const char* what() const throw();
    virtual cilt_Errno errno() const throw();
};

}

#endif // EXCEP_HPP
