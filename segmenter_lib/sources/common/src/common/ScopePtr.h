#ifndef COMMON_SCOPEPTR_H
#define COMMON_SCOPEPTR_H

#include <functional>

namespace vseg {

template <typename T>
class ScopePtr final {
public:
    ScopePtr(T *ptr, std::function<void(T*)> deallocator)
        : _ptr(ptr), _deleter(deallocator)
    {}

    ~ScopePtr() {
        if (_ptr) {
            _deleter(_ptr);
        }
    }

    ScopePtr(const ScopePtr &) = delete;
    ScopePtr & operator= (ScopePtr &) = delete;

    ScopePtr(ScopePtr && that)
        : _ptr(that._ptr), _deleter(that._deleter)
    {
        that._ptr = nullptr;
    }

    ScopePtr & operator= (ScopePtr && that) {
        if (this != &that)
        {
            _ptr = that._ptr;
            _deleter = that._deleter;
            that._ptr = nullptr;
        }
        return *this;
    }

    T * get() { return _ptr; }

    T * operator-> () {
        return _ptr;
    }

private:
    T *_ptr;
    std::function<void(T*)> _deleter;
};

}

#endif // COMMON_SCOPEPTR_H
