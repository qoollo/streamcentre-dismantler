#include "VideoSegmenter.h"

#include <iostream>
#include <common/CommonExceptions.h>
#include <common/IOExceptions.h>

#include <common/FileHelper.h>
#include "VideoFragment.h"
#include <video_segmenter/data_streams/VideoFileBuffer.h>
#include <video_segmenter/ffmpeg/FFmpegMuxer.h>
#include <video_segmenter/ffmpeg/FFmpegDemuxer.h>
#include <common/ScopePtr.h>
#include <chrono>
#include <boost/format.hpp>

extern "C" {
#include <libavformat/avformat.h>
}

namespace vseg {

bool VideoSegmenter::_ffmpegInitialized = false;

VideoSegmenter::VideoSegmenter(const std::string &url, int streamId, const SegmenterParameters &parameters)
: _parameters(parameters),
  _url(url),
  _isRunning(false),
  _streamId(streamId)
{
    _ffmpegDemuxer = std::make_shared<FFmpegDemuxer>(_url);
}

VideoSegmenter::~VideoSegmenter() {
    if (_runningThread.joinable()) {
        _runningThread.detach();
    }
}

void VideoSegmenter::openConnection() {
    if (!_ffmpegInitialized)
        initFFmpeg();

    _ffmpegDemuxer->openSource();
    _ffmpegMuxer = std::make_shared<FFmpegMuxer>(*_ffmpegDemuxer, StringHelper::toString(_parameters.containerFormat()));
}

void VideoSegmenter::closeConnection() {
    _ffmpegDemuxer->closeSource();
}

void VideoSegmenter::startAsync(std::function<void(sp<VideoFragment>)> newFragmentCallback) {
    if (_isRunning)
        throw WrongStateException("VideoSegmenter::startAsync(): segmenter is already running");

    if (!_ffmpegDemuxer->isOpen())
        throw WrongStateException("VideoSegmenter::startAsync(): connection closed");

    _isRunning = true;

    if (!_ffmpegDemuxer->hasVideoStream())
        throw BadSourceException("VideoSegmenter::startAsync(): cannot find video stream");

    _runningThread = std::thread(
        &VideoSegmenter::startSegmentation, this, newFragmentCallback);
}

void VideoSegmenter::start(std::function<void(sp<VideoFragment>)> newFragmentCallback) {
    if (_isRunning)
        throw WrongStateException("VideoSegmenter::startAsync(): segmenter is already running");

    if (!_ffmpegDemuxer->isOpen())
        throw WrongStateException("VideoSegmenter::startAsync(): connection closed");

    _isRunning = true;

    if (!_ffmpegDemuxer->hasVideoStream())
        throw BadSourceException("VideoSegmenter::startAsync(): cannot find video stream");

    startSegmentation(newFragmentCallback);
}

void VideoSegmenter::stop() {
    _stopFlag = true;
}

bool VideoSegmenter::isConnected() const {
    return _ffmpegDemuxer && _ffmpegDemuxer->isOpen();
}

void VideoSegmenter::startSegmentation(std::function<void(sp<VideoFragment>)> callback) {
    _stopFlag = false;
    bool decodeDone = false;

    double baseTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    double fragmentStartTime = baseTime;

    while (!_stopFlag && !decodeDone) {
        double duration = -1;
        sp<VideoBuffer> fragmentData = recordSegment(&decodeDone, &duration);

        callback(std::make_shared<VideoFragment>(
            fragmentData,
            _parameters.containerFormat(),
            _url, _streamId,
            fragmentStartTime, fragmentStartTime + duration));

        std::cout <<
            boost::format("start time: %1$0.2f ; end time: %2$0.2f") % fragmentStartTime % (fragmentStartTime + duration) <<
        std::endl;

        fragmentStartTime += duration;
    }

    _isRunning = false;
}

sp<VideoBuffer> VideoSegmenter::recordSegment(bool *decodeDone, double *duration) {
    auto currSegmentBuffer = std::make_shared<VideoFileBuffer>();
    _ffmpegMuxer->start(currSegmentBuffer);
    
    if (_ffmpegDemuxer->lastKeyFrame()) {
        AVPacket *lastKey = _ffmpegDemuxer->lastKeyFrame();
        setTimeStamps(lastKey);
        _ffmpegMuxer->writePacket(_ffmpegDemuxer->lastKeyFrame());
    }

    double recordTime = -1, startTime = -1;

    do {
        try {
            ScopePtr<AVPacket> packet(new AVPacket(), [] (AVPacket *p) { av_free_packet(p); });
            std::tuple<bool, FrameType, double> readResult = _ffmpegDemuxer->readFrame(packet.get());

            *decodeDone = std::get<0>(readResult);
            FrameType frameType = std::get<1>(readResult);
            double time = std::get<2>(readResult);

            if (frameType == FrameType::Video) {
                if (startTime == -1 && time > 0)
                    recordTime = startTime = time;

                setTimeStamps(packet.get());

                if (packet->flags & AV_PKT_FLAG_KEY || time - startTime > 2 * _parameters.fragmentTime())
                    recordTime = time;

                _ffmpegMuxer->writePacket(packet.get());
            }
        }
        catch(TimeoutException &e) {
            _stopFlag = true;
        }
    }
    while (!_stopFlag && (recordTime - startTime < _parameters.fragmentTime()) && !(*decodeDone));


    *duration = recordTime - startTime;
    _ffmpegMuxer->finish();

    return currSegmentBuffer;
}

void VideoSegmenter::wait() {
    if (_runningThread.joinable())
        _runningThread.join();
}

void VideoSegmenter::setTimeStamps(AVPacket* packet) {
    AVStream *inputStream = _ffmpegDemuxer->getContext()->streams[_ffmpegDemuxer->videoStreamIndex()];

    packet->pts = av_rescale_q_rnd(
        packet->pts,
        inputStream->time_base,
        _ffmpegMuxer->videoStream()->time_base,
        AVRounding(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

    packet->dts = av_rescale_q_rnd(
        packet->dts,
        inputStream->time_base,
        _ffmpegMuxer->videoStream()->time_base,
        AVRounding(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

    packet->duration = av_rescale_q(
        packet->duration,
        inputStream->time_base,
        _ffmpegMuxer->videoStream()->time_base);

    packet->pos = -1;
}

void VideoSegmenter::initFFmpeg() {
    av_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_DEBUG);
    _ffmpegInitialized = true;
}

}
