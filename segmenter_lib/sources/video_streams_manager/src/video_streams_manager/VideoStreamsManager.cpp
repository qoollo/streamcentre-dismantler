#include "VideoStreamsManager.h"
#include <video_saver/VideoSaver.h>
#include <video_saver/TestVideoSender.h>
#include <video_saver/http/HttpVideoSender.h>
#include <video_segmenter/SegmenterParameters.h>
#include "VideoRecorder.h"
#include "RecorderState.h"
#include <common/CommonExceptions.h>
#include <thread>
#include "configs/Configs.h"
#include "configs/ConfigReader.h"

namespace vseg {

VideoStreamsManager::VideoStreamsManager(const std::string &configsFile)
{
    ConfigReader configReader;
    _configs = configReader.readConfigs(configsFile);

    HttpVideoSender::Parameters senderParams(_configs->storageUrl());
    _saver = sp<IVideoSaver>(new VideoSaver(sp<IVideoSenderFactory>(new HttpVideoSenderFactory(senderParams))));
    _saver->start();
}

void VideoStreamsManager::startRecording(const std::string &url, int id) {
    if (_recorders.find(id) != _recorders.end())
        throw WrongStateException("VideoStreamsManager::startRecording(): stream already started");
    
    sp<VideoRecorder> recorder = std::make_shared<VideoRecorder>(url, id, _configs->segmenterParams(), _saver);
    
    recorder->start();
    
    std::lock_guard<std::mutex> guard(_mutex);
    _recorders[id] = recorder;
}

void VideoStreamsManager::stopRecording(int streamId) {
    if (_recorders.find(streamId) == _recorders.end()) {
        throw WrongStateException(
            "VideoStreamsManager::stopRecording(): no stream with id = "
            + StringHelper::toString(streamId) + " currently running");
    }

    _recorders[streamId]->stop();
    
    std::lock_guard<std::mutex> guard(_mutex);
    _recorders.erase(streamId);
}

RecorderState VideoStreamsManager::getRecordingState(int streamId) {
    if (_recorders.find(streamId) == _recorders.end()) {
        throw WrongStateException(
            "VideoStreamsManager::getRecordingState(): no stream with id = "
            + StringHelper::toString(streamId) + " currently running");
    }

    return _recorders[streamId]->state();
}

}
