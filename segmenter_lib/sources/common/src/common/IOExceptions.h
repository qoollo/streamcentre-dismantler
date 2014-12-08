#ifndef COMMON_IOEXCEPTIONS_H
#define COMMON_IOEXCEPTIONS_H

#include "BaseException.h"

namespace vseg {

class IOException : public BaseException {
public:
    explicit IOException(const std::string &message)
            : BaseException(message)
    {}
};

class ConnectionFailedException : public IOException {
public:
    explicit ConnectionFailedException(const std::string &message)
            : IOException(message)
    {}
};

class BadSourceException : public IOException {
public:
    explicit BadSourceException(const std::string &message)
            : IOException(message)
    {}
};

}

#endif // COMMON_IOEXCEPTIONS_H
