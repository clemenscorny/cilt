#ifndef EXCEP_HPP
#define EXCEP_HPP

#include <exception>
#include <string>

namespace cilt {

class Excep: public std::exception {
protected:
    std::string what_;
public:
    Excep() throw();
    Excep(const Excep& other) throw();
    Excep(const char* msg) throw();
    Excep(const std::string& msg) throw();
    virtual ~Excep() throw();
    const Excep& operator=(const Excep& other) throw();
    virtual const char* what() const throw();
};

};

#endif // EXCEP_HPP
