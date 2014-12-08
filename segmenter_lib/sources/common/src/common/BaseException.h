#ifndef COMMON_BASEEXCEPTION_H
#define COMMON_BASEEXCEPTION_H

#include <system_error>

namespace vseg {

class BaseException : public std::runtime_error {
public:
    explicit BaseException(const std::string &message)
        : runtime_error(message) 
    {}
};

}

#endif // COMMON_BASEEXCEPTION_H
