#ifndef VSEG_VIDEODATASTREAM_H
#define VSEG_VIDEODATASTREAM_H

#include <vector>
#include <stdint.h>

namespace vseg {

class VideoBuffer {
public:
    virtual ~VideoBuffer() {}

    virtual std::vector<char> getAllBytes() = 0;
};

}

#endif  // VSEG_VIDEODATASTREAM_H
