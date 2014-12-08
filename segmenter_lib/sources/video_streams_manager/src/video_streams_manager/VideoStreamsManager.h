#ifndef VSM_VIDEOSTREAMS_MANAGER_H
#define VSM_VIDEOSTREAMS_MANAGER_H

#include <common/VSCommon.h>
#include <string>
#include <map>
#include <mutex>
#include "configs/Configs.h"

namespace vseg {

class VideoRecorder;
class IVideoSaver;
class RecorderState;

class VideoStreamsManager {
public:
    VideoStreamsManager(const std::string &configsFile);

    void startRecording(const std::string &url, int id);
    void stopRecording(int id);
    RecorderState getRecordingState(int streamId);

private:
    std::map<int, sp<VideoRecorder>> _recorders;
    sp<IVideoSaver> _saver;
    std::mutex _mutex;
    up<Configs> _configs;
};

}

#endif // VSM_VIDEOSTREAMS_MANAGER_H
