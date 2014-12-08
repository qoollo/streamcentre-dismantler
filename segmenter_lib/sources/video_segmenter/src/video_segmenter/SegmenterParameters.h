#ifndef VSEG_SEGMENTERPARAMETERS_H
#define VSEG_SEGMENTERPARAMETERS_H

#include "ContainerFormats.h"
#include <map>
#include <string>

namespace vseg {

class SegmenterParameters {
public:
    SegmenterParameters(
        double fragmentTime,
        ContainerFormat containerFormat,
        int maxFragmentSize = -1);

    double fragmentTime() { return _fragmentTime;  }
    ContainerFormat containerFormat() { return _containerFormat; }
    int maxFragmentSize() { return _maxFragmentSize; }

private:
    double _fragmentTime;
    ContainerFormat _containerFormat;
    int _maxFragmentSize;

    static std::map<ContainerFormat, std::string> s_fmtToString;
};

}

#endif // VSEG_SEGMENTERPARAMETERS_H
