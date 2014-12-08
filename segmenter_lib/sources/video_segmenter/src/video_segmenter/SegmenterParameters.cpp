#include "SegmenterParameters.h"

namespace vseg {

SegmenterParameters::SegmenterParameters(
    double fragmentTime, 
    ContainerFormat containerFormat, 
    int maxFragmentSize)
: _fragmentTime(fragmentTime), 
  _containerFormat(containerFormat),
  _maxFragmentSize(maxFragmentSize)
{
}

}
