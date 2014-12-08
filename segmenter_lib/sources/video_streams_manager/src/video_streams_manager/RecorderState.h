#ifndef VSM_RECORDERSTATE_H
#define VSM_RECORDERSTATE_H

#include <ctime>
#include <string>

namespace vseg {

class RecorderState {
public:
    RecorderState();
    RecorderState(time_t startTime, bool running = false, bool connected = false, const std::string &statusMessage = "");

    bool running() const { return _running; }
    void setRunning(bool running) { _running = running; }

    bool connected() const { return _connected; }
    void setConnected(bool connected) { _connected = connected; }

    std::string statusMessage() { return _statusMessage; }
    void setStatusMessage(const std::string &value) { _statusMessage = value; }

    time_t startTime() const { return _startTime; }
    void setStartTime(const time_t &value) { _startTime = value; }

private:
    bool _running;
    bool _connected;
    time_t _startTime;
    std::string _statusMessage;
};

}

#endif // VSM_RECORDERSTATE_H
