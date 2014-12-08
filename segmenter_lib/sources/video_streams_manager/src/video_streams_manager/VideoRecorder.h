#ifndef VSM_VIDEORECORDER_H
#define VSM_VIDEORECORDER_H

#include <common/VSCommon.h>
#include <future>
#include <functional>
#include <mutex>
#include <video_segmenter/SegmenterParameters.h>
#include <video_segmenter/VideoFragment.h>
#include "RecorderState.h"

namespace vseg {

class VideoSegmenter;
class SegmenterParameters;
class IVideoSaver;

class VideoRecorder {
public:
    VideoRecorder(
            const std::string &sourceUrl,
            int streamId,
            const SegmenterParameters &segmenterParams,
            wp<IVideoSaver> saver);
    
    ~VideoRecorder();

    void start();
    void stop();
    RecorderState state();

private:
    sp<VideoSegmenter> _segmenter;
    wp<IVideoSaver> _saver;
    std::string _sourceUrl;
    int _streamId;
    RecorderState _state;
    volatile bool _stopped;
    std::thread _keepAliveThread;
    SegmenterParameters _segmenterParams;

    void startKeepAliveThread();
    void onNewFragment(sp<VideoFragment> fragment);
};

}

#endif // VSM_VIDEORECORDER_H