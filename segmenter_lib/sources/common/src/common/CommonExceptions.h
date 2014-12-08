#ifndef COMMON_COMMONEXCEPTIONS_H
#define COMMON_COMMONEXCEPTIONS_H

#include <string>
#include "BaseException.h"

namespace vseg {

class WrongArgumentException : public BaseException {
public:
    WrongArgumentException(const std::string &message)
            : BaseException(message)
    {}
};

class WrongStateException : public BaseException {
public:
    WrongStateException(const std::string &message)
            : BaseException(message)
    {}
};

class TimeoutException : public BaseException {
public:
    TimeoutException(const std::string &message)
            : BaseException(message)
    {}
};

}

#endif // COMMON_COMMONEXCEPTIONS_H