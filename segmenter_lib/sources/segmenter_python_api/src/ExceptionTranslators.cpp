#include "ExceptionTranslators.h"
#include <Python.h>

void ExceptionTranslator::translateBaseException(const vseg::BaseException &e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
}

void ExceptionTranslator::translateIOException(const vseg::IOException &e) {
    PyErr_SetString(PyExc_IOError, e.what());
}

void ExceptionTranslator::translateBadSourceException(const vseg::BadSourceException &e) {
    static PyObject *ex = PyErr_NewException("BadSourceException", PyExc_IOError, nullptr);
    PyErr_SetString(ex, e.what());
}

void ExceptionTranslator::translateConnectionFailedException(const vseg::ConnectionFailedException &e) {
    static PyObject *ex = PyErr_NewException("ConnectionFailedException", PyExc_IOError, nullptr);
    PyErr_SetString(ex, e.what());
}

void ExceptionTranslator::translateWrongArgumentException(const vseg::WrongArgumentException &e) {
    PyErr_SetString(PyExc_ValueError, e.what());
}

void ExceptionTranslator::translateWrongStateException(const vseg::WrongStateException &e) {
    static PyObject *ex = PyErr_NewException("WrongStateException", PyExc_RuntimeError, nullptr);
    PyErr_SetString(PyExc_ValueError, e.what());
}
