#include "FFmpegDemuxer.h"
#include <common/IOExceptions.h>
#include <common/CommonExceptions.h>

#include <iostream>

extern "C" {
#include <avformat.h>
}
namespace vseg {

FFmpegDemuxer::FFmpegDemuxer(const std::string &source)
: _url(source), _videoStreamIndex(-1), _lastKeyFrame(0), _isOpen(false)
{
}

FFmpegDemuxer::~FFmpegDemuxer() {
    if (_lastKeyFrame) {
        av_free_packet(_lastKeyFrame);
    }
}

void FFmpegDemuxer::openSource() {
    _fmtContext = avformat_alloc_context();
    
    AVIOInterruptCB intCb;
    intCb.opaque = this;
    intCb.callback = &FFmpegDemuxer::interruptCallback;
    _fmtContext->interrupt_callback = intCb;
    
    _timeoutDetected = false;
    _ongoingBlockingOperation = true;
    _blockOperationStartTime =  std::chrono::high_resolution_clock::now();

    if (avformat_open_input(&_fmtContext, _url.c_str(), 0, 0))
        throw ConnectionFailedException("FFmpegDemuxer::openSource(): connection failed");

    _ongoingBlockingOperation = false;
    
    if (_timeoutDetected)
        throw TimeoutException("FFmpeg::openSource(): timeout while trying to connect to stream " + _url);
        
    _isOpen = true;

    std::cout << "finding video stream info..." << std::endl;
    if (avformat_find_stream_info(_fmtContext, 0) < 0) {
        throw BadSourceException("FFmpegDemuxer::openSource(): failed to find stream info");
    }
    std::cout << "found video stream info" << std::endl;
    
    _videoStreamIndex = findVideoStreamIndex(_fmtContext);

    std::cout << _videoStreamIndex << std::endl;

    if (_videoStreamIndex < 0) {
        throw BadSourceException("FFmpegDemuxer::openSource(): no video stream found");
    }
}

int FFmpegDemuxer::findVideoStreamIndex(AVFormatContext *ctx) {
    int videoStream = -1;

    for (int i = 0; i < _fmtContext->nb_streams; i++) {
        if (ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    return videoStream;
}

void FFmpegDemuxer::closeSource() {
    if (_isOpen) {
        avformat_close_input(&_fmtContext);
    }
}

std::tuple<bool, FrameType, double> FFmpegDemuxer::readFrame(AVPacket* packet) {
    AVStream *videoStream = _fmtContext->streams[_videoStreamIndex];

    _timeoutDetected = false;
    _ongoingBlockingOperation = true;
    _blockOperationStartTime = std::chrono::high_resolution_clock::now();
    bool eof = av_read_frame(_fmtContext, packet);
    _ongoingBlockingOperation = false;
    
    if (_timeoutDetected) {
        std::cout << "FFmpegDemuxer::readFrame() timeout while trying to read frame from source " + _url << std::endl;
        throw TimeoutException("FFmpegDemuxer::readFrame() timeout while trying to read frame from source " + _url);
    }
    
    FrameType type = packet->stream_index == _videoStreamIndex ? FrameType::Video : FrameType::Unknown;
    double time = (type == FrameType::Video) ? (double) packet->pts * videoStream->time_base.num / videoStream->time_base.den : -1;

    if (type == FrameType::Video && packet->flags & AV_PKT_FLAG_KEY) {
        if (_lastKeyFrame)
            av_free_packet(_lastKeyFrame);

        _lastKeyFrame = (AVPacket*)av_malloc(sizeof(AVPacket));
        av_copy_packet(_lastKeyFrame, packet);
    }

    return std::tuple<bool, FrameType, double>(eof, type, time);
}

bool FFmpegDemuxer::hasVideoStream() const {
    return _videoStreamIndex != -1;
}

double FFmpegDemuxer::getVideoStreamTime() const {
    AVStream *videoStream = _fmtContext->streams[_videoStreamIndex];
    return videoStream->pts.val * videoStream->time_base.num / videoStream->time_base.den;
}

int FFmpegDemuxer::interruptCallback(void *opaque) {
    using namespace std::chrono;
    FFmpegDemuxer *instance = reinterpret_cast<FFmpegDemuxer*>(opaque);

    if (instance->_ongoingBlockingOperation) {
        auto time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(time - instance->_blockOperationStartTime);
        
        if (duration.count() > 5000) {
            instance->_timeoutDetected = true;
            return 1;
        }
        else {
            instance->_timeoutDetected = false;
        }
    }
    
    return 0;
}

}
