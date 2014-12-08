#ifndef VSEG_FFMPEGDEMUXER_H
#define VSEG_FFMPEGDEMUXER_H

#include <string>
#include <tuple>
#include <chrono>

struct AVPacket;
struct AVFormatContext;

namespace vseg {

enum class FrameType {
    Video, Audio, Unknown
};

class FFmpegDemuxer {
public:
    explicit FFmpegDemuxer(const std::string &source);
    ~FFmpegDemuxer();

    FFmpegDemuxer(const FFmpegDemuxer&) = delete;
    FFmpegDemuxer& operator= (const FFmpegDemuxer&) = delete;

    void openSource();
    void closeSource();

    std::tuple<bool, FrameType, double> readFrame(AVPacket *packet);
    double getVideoStreamTime() const;
    bool hasVideoStream() const;
    int videoStreamIndex() const { return _videoStreamIndex; }
    bool isOpen() const { return _isOpen; }

    const AVFormatContext * getContext() const { return _fmtContext; }
    AVFormatContext * getContext() { return _fmtContext; }
    AVPacket * lastKeyFrame() { return _lastKeyFrame; }

private:
    AVFormatContext *_fmtContext;
    bool _isOpen;
    std::string _url;
    int _videoStreamIndex;
    AVPacket *_lastKeyFrame;
    
    bool _timeoutDetected;
    bool _ongoingBlockingOperation;
    std::chrono::high_resolution_clock::time_point _blockOperationStartTime;
    

    int findVideoStreamIndex(AVFormatContext *ctx);
    
    static int interruptCallback(void *opaque);
};

}

#endif // VSEG_FFMPEGDEMUXER_H
