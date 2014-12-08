#ifndef VS_VIDEOSAVER_H
#define VS_VIDEOSAVER_H

#include <common/VSCommon.h>
#include <thread>
#include "IVideoSaver.h"
#include <common/concurrent/ConcurrentQueue.h>

namespace vseg {

class IVideoSenderFactory;

class VideoSaver : public IVideoSaver {
public:
    VideoSaver(sp<IVideoSenderFactory> sender);
    ~VideoSaver();
    
    void start() override;
    void stop() override;
    void wait() override;
    
    void saveFragment(sp<const VideoFragment> const& fragment) override;

private:
    sp<IVideoSenderFactory> _senderFactory;
    ConcurrentQueue<sp<const VideoFragment>> _queue;
    std::thread _sendThread;
    volatile bool _isStopped;
    
    void sendingThreadStart();
};

}

#endif // VS_VIDEOSAVER_H
