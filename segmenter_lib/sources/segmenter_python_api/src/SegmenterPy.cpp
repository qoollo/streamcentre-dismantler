#include <boost/python.hpp>
#include <video_streams_manager/VideoStreamsManager.h>
#include <video_streams_manager/RecorderState.h>
#include "ExceptionTranslators.h"
#include <string>
#include <memory>

std::shared_ptr<vseg::VideoStreamsManager> createStreamsManager(const std::string &configFileName) {
    return std::make_shared<vseg::VideoStreamsManager>(configFileName);
}

BOOST_PYTHON_MODULE(libsegmenter_py) {
    using namespace boost::python;
    using namespace vseg;

    boost::python::def("createStreamsManager", createStreamsManager);

    register_exception_translator<BaseException>(&ExceptionTranslator::translateBaseException);
    register_exception_translator<IOException>(&ExceptionTranslator::translateIOException);
    register_exception_translator<BadSourceException>(&ExceptionTranslator::translateBadSourceException);
    register_exception_translator<ConnectionFailedException>(&ExceptionTranslator::translateConnectionFailedException);
    register_exception_translator<WrongArgumentException>(&ExceptionTranslator::translateWrongArgumentException);
    register_exception_translator<WrongStateException>(&ExceptionTranslator::translateWrongStateException);
    
    class_<VideoStreamsManager, boost::noncopyable, std::shared_ptr<VideoStreamsManager>>("Segmenter", no_init)
        .def("start_recording", &VideoStreamsManager::startRecording)
        .def("stop_recording", &VideoStreamsManager::stopRecording)
        .def("get_state", &VideoStreamsManager::getRecordingState);

    class_<RecorderState>("RecorderState")
        .add_property("is_running", &RecorderState::running, &RecorderState::setRunning)
        .add_property("is_connected", &RecorderState::connected, &RecorderState::setConnected)
        .add_property("start_time", &RecorderState::startTime)
        .add_property("message", &RecorderState::statusMessage);
}
