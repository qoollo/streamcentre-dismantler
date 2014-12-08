#ifndef VSEG_VIDEOFRAGMENT
#define VSEG_VIDEOFRAGMENT

#include <common/VSCommon.h>
#include "ContainerFormats.h"
#include <vector>
#include <ctime>
#include <string>

namespace vseg {

class VideoBuffer;

class VideoFragment {
public:
    VideoFragment(
        sp<VideoBuffer> data,
        ContainerFormat containerFormat,
        const std::string &sourceUrl,
        int streamId,
        time_t startTime,
        time_t endTime);

    ContainerFormat container() const { return _containerFormat; }
    sp<VideoBuffer> data() const { return _data; }
    time_t startTime() const { return _startTime; }
    time_t endTime() const { return _endTime; }
    std::string const& sourceUrl() const { return _sourceUrl; }
    int streamId() const { return _streamId; }

private:
    ContainerFormat _containerFormat;
    time_t _startTime;
    time_t _endTime;
    sp<VideoBuffer> _data;
    std::string _sourceUrl;
    int _streamId;
};

}

#endif // VSEG_VIDEOFRAGMENT
