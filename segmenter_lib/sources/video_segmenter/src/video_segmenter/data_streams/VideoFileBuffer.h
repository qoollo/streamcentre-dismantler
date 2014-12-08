#ifndef VSEG_VIDEOFILEBUFFER_H
#define VSEG_VIDEOFILEBUFFER_H

#include "VideoBuffer.h"
#include "IFFMpegIOStream.h"
#include <stdio.h>
#include <fstream>

namespace vseg {

class VideoFileBuffer : public VideoBuffer, public IFFMpegIOStream {
public:
    explicit VideoFileBuffer(const std::string &fileName);
    explicit VideoFileBuffer();
    ~VideoFileBuffer();

    std::vector<char> getAllBytes() override;
    
    int read(uint8_t* buf, int buf_size) override;
    int write(uint8_t* begin, int size) override;
    int64_t seek(int64_t pos, int whence) override;

private:
    std::string createUniqueFileName();
    std::string _fileName;
    FILE *_fileDesc;
};

}

#endif // VIDEOFILEBUFFER_H

