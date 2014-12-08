#ifndef VS_TESTVIDEOSAVER_H
#define VS_TESTVIDEOSAVER_H

#include "IVideoSender.h"
#include <atomic>

namespace vseg {

class TestVideoSender : public IVideoSender {
public:
    TestVideoSender();

    virtual bool sendFragment(sp<const VideoFragment> const &fragment);

private:
    std::atomic<int> _counter;
};

class TestVideoSenderFactory : public IVideoSenderFactory {
public:
    virtual sp<IVideoSender> createSender();
};

}

#endif // VS_TESTVIDEOSAVER_H
