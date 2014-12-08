#include "RecorderState.h"

namespace vseg {

RecorderState::RecorderState()
: _running(false), _connected(false), _startTime(0)
{
}

RecorderState::RecorderState(time_t startTime, bool running, bool connected, const std::string &statusMessage)
: _running(running), _connected(connected), _startTime(startTime),  _statusMessage(statusMessage)
{
}

}
