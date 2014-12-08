#ifndef VS_IVIDEOSAVER_H
#define VS_IVIDEOSAVER_H

#include <common/VSCommon.h>

namespace vseg {

class VideoFragment;

class IVideoSaver {
public:
    virtual ~IVideoSaver() {}
    
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void wait() = 0;
    
    virtual void saveFragment(sp<const VideoFragment> const& fragment) = 0;
};

}

#endif // VS_IVIDEOSAVER_H