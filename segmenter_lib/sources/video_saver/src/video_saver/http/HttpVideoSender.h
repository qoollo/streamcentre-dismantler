#ifndef VS_HTTPVIDEOSENDER_H
#define VS_HTTPVIDEOSENDER_H

#include <video_saver/IVideoSender.h>
#include <string>

namespace vseg {

class HttpVideoSender : public IVideoSender {
public:
    class Parameters {
    public:
        Parameters(const std::string &remoteUrl);

        std::string const& remoteUrl() { return _remoteUrl; }

    private:
        std::string _remoteUrl;
    };

    HttpVideoSender(const Parameters &parameters);

    virtual bool sendFragment(sp<const VideoFragment> const &fragment);

private:
    Parameters _params;
};

class HttpVideoSenderFactory : public IVideoSenderFactory {
public:
    HttpVideoSenderFactory(const HttpVideoSender::Parameters &senderParameters);

    virtual sp<IVideoSender> createSender();

private:
    HttpVideoSender::Parameters _senderParameters;
};

}

#endif
