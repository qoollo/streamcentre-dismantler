#include <iostream>
#include <sstream>
#include <boost/iostreams/stream.hpp>
#include <common/BaseException.h>
#include <video_streams_manager/VideoStreamsManager.h>
#include <thread>
#include <common/concurrent/ConcurrentQueue.h>
#include <future>

using namespace vseg;

std::mutex coutMutex;

void concurrentQueueTest();
void cqWriter(ConcurrentQueue<int> &cq, int id);
void cqReader(ConcurrentQueue<int> &cq, int id);

int main(int argc, char **argv) {
    try {
        VideoStreamsManager segmenter("configs.json");
//        segmenter.startRecording("rtsp://10.5.5.81:554/ONVIF/MediaInput?profile=h264", 1);
//        segmenter.startRecording("rtsp://127.0.0.1:8008/camera.sdp", 2);
        segmenter.startRecording("rtsp://10.5.5.232:8050/mosaic.sdp", 3);
        std::cin.get();
        segmenter.stopRecording(3);
    }
    catch (BaseException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

void concurrentQueueTest() {
    ConcurrentQueue<int> cq;

    std::thread writeThread1(
            std::bind(cqWriter, std::ref(cq), 1));

    std::thread writeThread2(
            std::bind(cqWriter, std::ref(cq), 2));

    std::thread writeThread3(
            std::bind(cqWriter, std::ref(cq), 3));

    std::thread readThread1(
            std::bind(cqReader, std::ref(cq), 1));

    std::thread readThread2(
            std::bind(cqReader, std::ref(cq), 2));

    std::thread readThread3(
            std::bind(cqReader, std::ref(cq), 3));

    writeThread1.join();
    writeThread2.join();
    writeThread3.join();
}

void cqWriter(ConcurrentQueue<int> &cq, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    while (true) {
        coutMutex.lock();
        std::cout << "writer " << id << ": pushing" << std::endl;
        coutMutex.unlock();
        cq.push(123);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void cqReader(ConcurrentQueue<int> &cq, int id) {
    while (true) {
        coutMutex.lock();
        std::cout << "reader " << id << ": waiting.." << std::endl;
        coutMutex.unlock();
        int a = cq.pop();

        coutMutex.lock();
        std::cout << "reader " << id <<  ": read value " << a << std::endl;
        std::cout << "queue size: " << cq.size() << std::endl;
    }
}
