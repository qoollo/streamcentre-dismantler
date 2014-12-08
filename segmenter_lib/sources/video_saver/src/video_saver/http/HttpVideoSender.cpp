#include "HttpVideoSender.h"

#include <common/ScopePtr.h>
#include <video_segmenter/VideoFragment.h>
#include <video_segmenter/data_streams/VideoBuffer.h>
#include "HttpFormRequest.h"
#include <video_segmenter/ContainerFormats.h>
#include <vector>

namespace vseg {

HttpVideoSender::HttpVideoSender(HttpVideoSender::Parameters const &parameters)
: _params(parameters)
{

}

bool HttpVideoSender::sendFragment(sp<const VideoFragment> const &fragment) {
    HttpFormRequest request;

    request.addString("stream_id", StringHelper::toString(fragment->streamId()));
    request.addString("container_format", StringHelper::toString(fragment->container()));
    request.addString("start_time", StringHelper::toString(fragment->startTime()));
    request.addString("stop_time", StringHelper::toString(fragment->endTime()));
    
    auto data = fragment->data()->getAllBytes();
    request.addBinary("data", data);
    
    int code = request.perform(_params.remoteUrl());

    std::cout << _params.remoteUrl() << std::endl;

    return code == 200;
}

sp<IVideoSender> HttpVideoSenderFactory::createSender() {
    return sp<IVideoSender>(new HttpVideoSender(_senderParameters));
}

HttpVideoSender::Parameters::Parameters(const std::string &remoteUrl)
: _remoteUrl(remoteUrl)
{
}

HttpVideoSenderFactory::HttpVideoSenderFactory(const HttpVideoSender::Parameters &senderParameters)
: _senderParameters(senderParameters)
{
}

}
