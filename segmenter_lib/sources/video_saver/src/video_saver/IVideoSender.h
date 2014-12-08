#ifndef VS_IVIDEOSENDER_H
#define VS_IVIDEOSENDER_H

#include <common/VSCommon.h>

namespace vseg {

class VideoFragment;

class IVideoSender {
public:
    virtual ~IVideoSender() {}
    
    virtual bool sendFragment(sp<const VideoFragment> const &fragment) = 0;
};

class IVideoSenderFactory {
public:
    virtual ~IVideoSenderFactory() {}

    virtual sp<IVideoSender> createSender() = 0;
};

}

#endif // VS_IVIDEOSENDER_H

