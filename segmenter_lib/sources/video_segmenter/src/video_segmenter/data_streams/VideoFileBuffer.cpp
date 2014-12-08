#include "VideoFileBuffer.h"
#include <sstream>
#include <fstream>
#include <common/FileHelper.h>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace vseg {

VideoFileBuffer::VideoFileBuffer(const std::string &fileName)
    : _fileName(fileName), _fileDesc(0)
{
    _fileDesc = fopen(_fileName.c_str(), "wb");
}

VideoFileBuffer::VideoFileBuffer()
{
    _fileName = createUniqueFileName();
    _fileDesc = fopen(_fileName.c_str(), "wb");
}

VideoFileBuffer::~VideoFileBuffer() {
    if (_fileDesc) {
        fclose(_fileDesc);
        remove(_fileName.c_str());
    }
}

std::vector<char> VideoFileBuffer::getAllBytes() {
    if (_fileDesc)
        fflush(_fileDesc);

    return FileHelper::readAllByes(_fileName);
}

std::string VideoFileBuffer::createUniqueFileName() {
//    boost::uuids::basic_random_generator<boost::mt19937> gen;
//    boost::uuids::uuid u = gen();

    return boost::lexical_cast<std::string>((uint64_t) this);
}

int VideoFileBuffer::read(uint8_t* buf, int buf_size) {
    return fread(buf, 1, buf_size, _fileDesc);
}

int VideoFileBuffer::write(uint8_t* begin, int size) {
    return fwrite(begin, 1, size, _fileDesc);
}

int64_t VideoFileBuffer::seek(int64_t pos, int whence) {
    return fseek(_fileDesc, pos, whence);
}

}
