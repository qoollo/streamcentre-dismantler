#include "Configs.h"

namespace vseg {

Configs::Configs(SegmenterParameters const& segmenterParams,
                 std::string const& storageUrl)
: _segmenterParams(segmenterParams),
  _storageUrl(storageUrl)
{
}

}
