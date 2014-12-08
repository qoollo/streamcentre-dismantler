#ifndef VSEG_FFMPEGMUXER_H
#define VSEG_FFMPEGMUXER_H

#include <common/VSCommon.h>
#include <string>
#include <map>
#include <video_segmenter/ContainerFormats.h>
#include <cinttypes>

struct AVFormatContext;
struct AVPacket;
struct AVStream;

namespace vseg {

class VideoBuffer;
class IFFMpegIOStream;
class FFmpegDemuxer;

class FFmpegMuxer {
public:
    explicit FFmpegMuxer(const FFmpegDemuxer &demuxer, const std::string &containerFormat);
    ~FFmpegMuxer();

    void writePacket(AVPacket *packet);
    void writePacket(AVPacket *packet, int pts);

    void start(sp<IFFMpegIOStream> ioStream);
    void finish();

    AVStream * videoStream();
    AVFormatContext *context();

private:
    static const int INTERNAL_BUFFER_SIZE;

    AVFormatContext *_fmtContext;
    sp<IFFMpegIOStream> _stream;
    unsigned char *_internalBuffer;
    int _videoStreamIdx;
    bool _firstWrite;

    void allocAVIOContext();
    AVStream* addOutputStream(AVStream *inputStream);
    void logPacket(AVPacket *packet);

    static int customRead(void *opaque, uint8_t *buf, int buf_size);
    static int customWrite(void *opaque, uint8_t *begin, int buf_size);
    static int64_t customSeek(void *opaque, int64_t pos, int whence);
};

}

#endif // VSEG_FFMPEGMUXER_H

