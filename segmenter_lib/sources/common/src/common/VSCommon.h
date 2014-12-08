#ifndef COMMON_VSCOMMON_H
#define COMMON_VSCOMMON_H

#include <memory>

namespace vseg {

using uchar = unsigned char;

template <typename T>
using sp = std::shared_ptr<T>;

template <typename T>
using wp = std::weak_ptr<T>;

template <typename T, typename Deleter = std::default_delete<T>>
using up = std::unique_ptr<T, Deleter>;

}

#endif // COMMON_VSCOMMON_H
