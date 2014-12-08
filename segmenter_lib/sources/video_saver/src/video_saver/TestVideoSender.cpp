#include "TestVideoSender.h"

#include <common/FileHelper.h>
#include <common/StringHelper.h>
#include <video_segmenter/VideoFragment.h>
#include <video_segmenter/data_streams/VideoBuffer.h>

namespace vseg {


TestVideoSender::TestVideoSender() {

}

sp<IVideoSender> TestVideoSenderFactory::createSender() {
    return sp<IVideoSender>(new TestVideoSender());
}

bool TestVideoSender::sendFragment(sp<const VideoFragment> const &fragment) {
    FileHelper::writeAllBytes(
        StringHelper::toString(fragment->streamId()) + "_" +
        StringHelper::toString((++_counter)) +
        ".mkv",
        fragment->data()->getAllBytes());

    return true;
}

}
