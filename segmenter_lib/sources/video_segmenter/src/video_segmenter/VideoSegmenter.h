#ifndef VSEG_VIDEOSEGMENTER_H
#define VSEG_VIDEOSEGMENTER_H

#include <common/VSCommon.h>
#include <string>
#include <functional>
#include <future>
#include <chrono>

#include "SegmenterParameters.h"

struct AVFormatContext;
struct AVStream;
struct AVPacket;

namespace vseg {

class VideoBuffer;
class VideoFragment;
class FFmpegMuxer;
class FFmpegDemuxer;

class VideoSegmenter {
public:
    VideoSegmenter(const std::string &url, int streamId, const SegmenterParameters &parameters);
    ~VideoSegmenter();

    VideoSegmenter(const VideoSegmenter&) = delete;
    VideoSegmenter& operator= (const VideoSegmenter&) = delete;

    void openConnection();
    void closeConnection();
    
    void wait();

    void startAsync(std::function<void(sp<VideoFragment>)> newFragmentCallback);
    void start(std::function<void(sp<VideoFragment>)> newFragmentCallback);
    void stop();

    bool isRunning() const { return _isRunning; }
    bool isConnected() const;

private:
    SegmenterParameters _parameters;
    std::string _url;
    
    sp<FFmpegDemuxer> _ffmpegDemuxer;
    sp<FFmpegMuxer> _ffmpegMuxer;

    std::thread _runningThread;

    volatile bool _isRunning;
    volatile bool _stopFlag;
    
    int _streamId;

    void startSegmentation(std::function<void(sp<VideoFragment>)> callback);
    sp <VideoBuffer> recordSegment(bool *decodeDone, double *duration);
    void setTimeStamps(AVPacket *packet);

    // static fields
    static bool _ffmpegInitialized;
    static void initFFmpeg();
};

}

#endif // VSEG_VIDEOSEGMENTER_H
