#ifndef VSM_CONFIGS_H
#define VSM_CONFIGS_H

#include <common/VSCommon.h>
#include <video_segmenter/SegmenterParameters.h>
#include <video_segmenter/ContainerFormats.h>

namespace vseg {

class Configs {
public:
    Configs(
        SegmenterParameters const& segmenterParams,
        std::string const& storageUrl
    );

    SegmenterParameters const& segmenterParams() { return _segmenterParams; }
    std::string const& storageUrl() { return _storageUrl; }

private:
    SegmenterParameters _segmenterParams;
    std::string _storageUrl;
};

}

#endif // VSM_CONFIGS_H
