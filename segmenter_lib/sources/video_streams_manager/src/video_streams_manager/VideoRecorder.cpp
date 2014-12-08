#include "VideoRecorder.h"

#include <chrono>

#include <video_segmenter/VideoSegmenter.h>
#include <video_segmenter/VideoFragment.h>
#include <video_saver/IVideoSaver.h>
#include <common/BaseException.h>
#include <iostream>

namespace vseg {

VideoRecorder::VideoRecorder(const std::string &sourceUrl, int streamId, const SegmenterParameters &segmenterParams, wp<IVideoSaver> saver)
: _saver(saver),
  _sourceUrl(sourceUrl),
  _streamId(streamId),
  _stopped(true),
  _segmenterParams(segmenterParams)
{
}

VideoRecorder::~VideoRecorder() {
    if (_keepAliveThread.joinable())
        _keepAliveThread.detach();
}

void VideoRecorder::start() {
    _stopped = false;
    _keepAliveThread = std::thread(&VideoRecorder::startKeepAliveThread, this);
}

void VideoRecorder::stop() {
    _stopped = true;
    if (_segmenter) {
        _segmenter->stop();
        _segmenter->wait();
        _segmenter->closeConnection();
    }
    
    if (_keepAliveThread.joinable())
        _keepAliveThread.join();
}

RecorderState VideoRecorder::state() {
    _state.setRunning(_segmenter->isRunning());
    _state.setConnected(_segmenter->isConnected());

    return _state;
}

void VideoRecorder::startKeepAliveThread() {
    while (!_stopped) {
        try {
            if (_segmenter && (!_segmenter->isConnected() || !_segmenter->isRunning())) {
                _segmenter->stop();
                _segmenter->closeConnection();
                _segmenter->wait();
                _segmenter = std::make_shared<VideoSegmenter>(_sourceUrl, _streamId, _segmenterParams);
                _segmenter->openConnection();
                _segmenter->startAsync(std::bind(&VideoRecorder::onNewFragment, this, std::placeholders::_1));
            }
            
            if (!_segmenter) {
                _segmenter = std::make_shared<VideoSegmenter>(_sourceUrl, _streamId, _segmenterParams);
                _segmenter->openConnection();
                _segmenter->startAsync(std::bind(&VideoRecorder::onNewFragment, this, std::placeholders::_1));
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        catch (BaseException &e) {
            std::string message =
                    "Error: failed to connect to stream id " +
                    StringHelper::toString(_streamId) +
                    ". Internal error message: " + e.what();

            _state.setStatusMessage(message);
            std::cout << message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void VideoRecorder::onNewFragment(sp<VideoFragment> fragment) {
    auto saver = _saver.lock();
    if (saver) {
        saver->saveFragment(fragment);
    }
}

}
