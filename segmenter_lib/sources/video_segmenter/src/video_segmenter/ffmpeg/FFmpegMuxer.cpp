#include "FFmpegMuxer.h"
#include <common/IOExceptions.h>
#include <video_segmenter/data_streams/IFFMpegIOStream.h>
#include <video_segmenter/ffmpeg/FFmpegDemuxer.h>
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
}

namespace vseg {

const int FFmpegMuxer::INTERNAL_BUFFER_SIZE = 1 * 1024 * 1024;

FFmpegMuxer::FFmpegMuxer(const FFmpegDemuxer &demuxer, const std::string & containerFormat)
    :   _fmtContext(0),
        _stream(0),
        _internalBuffer(0),
        _firstWrite(false)
{
    _internalBuffer = new unsigned char[INTERNAL_BUFFER_SIZE];
    
    _fmtContext = avformat_alloc_context();
    _fmtContext->oformat = av_guess_format(containerFormat.c_str(), ("temp." + containerFormat).c_str(), 0);
    addOutputStream(demuxer.getContext()->streams[demuxer.videoStreamIndex()]);
}

FFmpegMuxer::~FFmpegMuxer() {
    if (_fmtContext) {
//        avformat_free_context(_fmtContext);
    }

    if (_internalBuffer)
        delete[] _internalBuffer;
}

void FFmpegMuxer::writePacket(AVPacket *packet) {
    if (_firstWrite) {
//        _fmtContext->streams[_videoStreamIdx]->start_time = packet->pts;
        _firstWrite = false;
    }

    packet->stream_index = _videoStreamIdx;
    av_interleaved_write_frame(_fmtContext, packet);
}

void FFmpegMuxer::writePacket(AVPacket *packet, int pts) {
    packet->pts = packet->dts = pts;
    av_write_frame(_fmtContext, packet);
}

void FFmpegMuxer::start(sp<IFFMpegIOStream> ioStream) {
    _stream = ioStream;
    allocAVIOContext();
    avformat_write_header(_fmtContext, 0);
    _firstWrite = true;
}

void FFmpegMuxer::finish() {
    av_write_trailer(_fmtContext);
    avio_flush(_fmtContext->pb);
    _fmtContext->pb = 0;
}

AVStream * FFmpegMuxer::videoStream() {
    return _fmtContext->streams[_videoStreamIdx];
}

AVFormatContext* FFmpegMuxer::context() {
    return _fmtContext;
}

void FFmpegMuxer::allocAVIOContext() {
    AVIOContext *avioc = avio_alloc_context(
        _internalBuffer,
        INTERNAL_BUFFER_SIZE,
        AVIO_FLAG_WRITE,
        _stream.get(),
        customRead,
        customWrite,
        customSeek);

    _fmtContext->pb = avioc;
}

int FFmpegMuxer::customRead(void *opaque, uint8_t *buf, int buf_size) {
    IFFMpegIOStream *stream = static_cast<IFFMpegIOStream*>(opaque);
    return stream->read(buf, buf_size);
}

int FFmpegMuxer::customWrite(void *opaque, uint8_t *begin, int buf_size) {
    IFFMpegIOStream *stream = static_cast<IFFMpegIOStream*>(opaque);
    return stream->write(begin, buf_size);
}

int64_t FFmpegMuxer::customSeek(void *opaque, int64_t pos, int whence) {
    IFFMpegIOStream *stream = static_cast<IFFMpegIOStream*>(opaque);
    return stream->seek(pos, whence);
}

AVStream* FFmpegMuxer::addOutputStream(AVStream *inputStream) {
    AVStream *outStream = avformat_new_stream(_fmtContext, 0);
    avcodec_copy_context(outStream->codec, inputStream->codec);
    outStream->codec->codec_tag = 0;
    if (_fmtContext->flags & AVFMT_GLOBALHEADER) {
        outStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }
    _videoStreamIdx = 0;
    return outStream;
}

void FFmpegMuxer::logPacket(AVPacket* packet) {
    AVStream *videoStream = _fmtContext->streams[_videoStreamIdx];
    double ptime = (double) packet->pts * videoStream->time_base.num / videoStream->time_base.den;
    double dtime = (double) packet->dts * videoStream->time_base.num / videoStream->time_base.den;
    double time = (double) videoStream->start_time * videoStream->time_base.num / videoStream->time_base.den;
    std::cout <<
        "packet: time: " << ptime <<
        " . dtime: " << dtime <<
        "start time: " << time <<
        ((packet->flags & AV_PKT_FLAG_KEY) ? "    ======== KEY FRAME ========" : "") <<
        std::endl;
}

}
