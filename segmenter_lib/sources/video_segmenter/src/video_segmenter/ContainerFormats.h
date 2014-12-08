#ifndef VSEG_CONTAINERFORMATS_H
#define VSEG_CONTAINERFORMATS_H

#include <common/StringHelper.h>
#include <iostream>

namespace vseg {

enum class ContainerFormat {
    MKV = 0,
    MP4 = 1,
    AVI = 2,
    DEFAULT = 0
};

template <>
inline std::string StringHelper::toString<ContainerFormat>(ContainerFormat value) {
    switch (value) {
        case ContainerFormat::MKV: return "mkv";
        case ContainerFormat::AVI: return "avi";
        case ContainerFormat::MP4: return "mp4";
    }
}

template<>
inline ContainerFormat StringHelper::fromString<ContainerFormat>(const std::string &s) {
    ContainerFormat res = ContainerFormat::DEFAULT;
    
    if (s == "mkv")
        res = ContainerFormat::MKV;
    else if (s == "avi")
        res = ContainerFormat::AVI;
    else if (s == "mp4")
        res = ContainerFormat::MP4;
    
    return res;
}

}

#endif // VSEG_CONTAINERFORMATS_H
