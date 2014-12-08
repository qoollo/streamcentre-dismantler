#ifndef VSEG_IFFMPEGIOSTREAM_H
#define VSEG_IFFMPEGIOSTREAM_H

#include <stdint.h>

namespace vseg {
    
class IFFMpegIOStream {
public:
    virtual ~IFFMpegIOStream() {}

    virtual int read(uint8_t *buf, int buf_size) = 0;
    virtual int write(uint8_t *begin, int size) = 0;
    virtual int64_t seek(int64_t pos, int whence) = 0;
};

}

#endif // VSEG_IFFMPEGIOSTREAM_H

