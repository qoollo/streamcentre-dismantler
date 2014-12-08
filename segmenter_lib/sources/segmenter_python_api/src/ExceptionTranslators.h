#ifndef PY_SEGMENTERTRANSLATORS_H
#define PY_SEGMENTERTRANSLATORS_H

#include <common/BaseException.h>
#include <common/IOExceptions.h>
#include <common/CommonExceptions.h>
#include <map>
#include <Python.h>


class ExceptionTranslator {
public:
    static void translateBaseException(const vseg::BaseException &e);
    static void translateIOException(const vseg::IOException &e);
    static void translateBadSourceException(const vseg::BadSourceException &e);
    static void translateConnectionFailedException(const vseg::ConnectionFailedException &e);
    static void translateWrongArgumentException(const vseg::WrongArgumentException &e);
    static void translateWrongStateException(const vseg::WrongStateException &e);
};

#endif // PY_SEGMENTERTRANSLATORS_H