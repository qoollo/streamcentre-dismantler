#include "LocalVideoSaver.h"
#include <common/FileHelper.h>
#include <common/StringHelper.h>
#include <video_segmenter/data_streams/VideoBuffer.h>

namespace vseg {

void LocalVideoSaver::saveFragment(sp<const VideoFragment> const& fragment) {
    FileHelper::writeAllBytes(
            StringHelper::toString(fragment->streamId()) + "_" +
            StringHelper::toString((++_counter)) +
            ".mkv",
        fragment->data()->getAllBytes());
}

void LocalVideoSaver::start() {
}

void LocalVideoSaver::stop() {
}

void LocalVideoSaver::wait() {
}

}
