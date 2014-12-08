#ifndef COMMON_STRINGHELPER_H
#define COMMON_STRINGHELPER_H

#include <string>
#include <boost/lexical_cast.hpp>

namespace vseg {

class StringHelper {
public:
    template <typename T>
    static std::string toString(T num) {
        return boost::lexical_cast<std::string>(num);
    }

    template <typename T>
    static T fromString(const std::string &s) {
        return boost::lexical_cast<T>(s);
    }
};

}

#endif // COMMON_STRINGHELPER_H
