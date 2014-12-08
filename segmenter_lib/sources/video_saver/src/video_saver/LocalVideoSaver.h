#ifndef VIDEOSAVER_VIDEOSAVER_H
#define VIDEOSAVER_VIDEOSAVER_H

#include <common/VSCommon.h>
#include <video_segmenter/VideoFragment.h>
#include <atomic>
#include "IVideoSaver.h"

namespace vseg {

class LocalVideoSaver : public IVideoSaver {
public:
    void saveFragment(sp<const VideoFragment> const& fragment) override;
    
    void start() override;
    void stop() override;
    void wait() override;

private:
    std::atomic<int> _counter;
};

}

#endif // VIDEOSAVER_VIDEOSAVER_H