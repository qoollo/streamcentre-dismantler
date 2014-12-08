#include "VideoSaver.h"
#include "IVideoSender.h"
#include <future>
#include <iostream>

namespace vseg {

VideoSaver::VideoSaver(sp<IVideoSenderFactory> senderFactory)
: _senderFactory(senderFactory)
{
}

VideoSaver::~VideoSaver() {
    stop();
    if (_sendThread.joinable()) {
        _sendThread.detach();
    }
}

void VideoSaver::saveFragment(sp<const VideoFragment> const& fragment) {
    _queue.push(fragment);
}

void VideoSaver::start() {
    _isStopped = false;
    _sendThread = std::thread(std::bind(&VideoSaver::sendingThreadStart, this));
}

void VideoSaver::stop() {
    _isStopped = true;
}

void VideoSaver::wait() {
    if (_sendThread.joinable()) {
        _sendThread.join();
    }
}

void VideoSaver::sendingThreadStart() {
    sp<IVideoSender> sender = _senderFactory->createSender();

    while (!_isStopped) {
        auto fragment = _queue.pop();
        if (!sender->sendFragment(fragment)) {
            std::cout << "failed to send fragment to storage" << std::endl;
        }
    }
}

}
