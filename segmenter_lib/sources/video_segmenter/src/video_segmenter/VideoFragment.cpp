#include "VideoFragment.h"

#include <video_segmenter/data_streams/VideoBuffer.h>

namespace vseg {

VideoFragment::VideoFragment(
    sp<VideoBuffer> data,
    ContainerFormat containerFormat,
    const std::string &sourceUrl,
    int streamId,
    time_t startTime,
    time_t endTime)
    :  _containerFormat(containerFormat),
       _startTime(startTime),
       _endTime(endTime),
       _data(data),
       _sourceUrl(sourceUrl),
       _streamId(streamId)
{
}

}
