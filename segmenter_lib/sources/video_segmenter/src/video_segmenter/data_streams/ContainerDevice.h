#ifndef VSEG_CONTAINERDEVICE_H
#define VSEG_CONTAINERDEVICE_H

#include <iostream>
#include <boost/iostreams/seek.hpp>

namespace vseg {

namespace io = boost::iostreams;

template<typename Container>
class ContainerDevice {
public:
    typedef typename Container::value_type char_type;
    typedef io::seekable_device_tag category;

    explicit ContainerDevice(Container &container)
        : container_(container), pos_(0)
    { }

    std::streamsize read(char_type* s, std::streamsize n) {
        using namespace std;
        streamsize amt = static_cast<streamsize>(container_.size() - pos_);
        streamsize result = (min)(n, amt);
        if (result != 0) {
            std::copy(container_.begin() + pos_,
                container_.begin() + pos_ + result,
                s);
            pos_ += result;
            return result;
        }
        else {
            return -1; // EOF
        }
    }

    std::streamsize write(const char_type* s, std::streamsize n) {
        using namespace std;
        streamsize result = 0;
        if (pos_ != container_.size()) {
            streamsize amt =
                static_cast<streamsize>(container_.size() - pos_);
            result = (min)(n, amt);
            std::copy(s, s + result, container_.begin() + pos_);
            pos_ += result;
        }
        if (result < n) {
            container_.insert(container_.end(), s + result, s + n);
            pos_ = container_.size();
        }
        return n;
    }

    io::stream_offset seek(io::stream_offset off, std::ios_base::seekdir way) {
        using namespace std;

        // Determine new value of pos_
        io::stream_offset next;
        if (way == ios_base::beg) {
            next = off;
        }
        else if (way == ios_base::cur) {
            next = pos_ + off;
        }
        else if (way == ios_base::end) {
            next = container_.size() + off - 1;
        }
        else {
            throw ios_base::failure("bad seek direction");
        }

        // Check for errors
        if (next < 0 || next >= container_.size())
            throw ios_base::failure("bad seek offset");

        pos_ = next;
        return pos_;
    }

    Container& container() { return container_; }

private:
    typedef typename Container::size_type size_type;
    Container&  container_;
    size_type   pos_;
};

}

#endif // VSEG_CONTAINERDEVICE_H

