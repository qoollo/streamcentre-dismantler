#ifndef COMMON_CONCURRENTQUEUE_H
#define COMMON_CONCURRENTQUEUE_H

#include <queue>
#include <condition_variable>

namespace vseg {

template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue()
    {}

    void push(const T &item) {
        std::unique_lock<std::mutex> lock(_mutex);
        _queue.push(item);
        lock.unlock();
        _condVar.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) {
            _condVar.wait(lock);
        }

        auto item = _queue.front();
        _queue.pop();
        return item;
    }
    
    int size() { return _queue.size(); }

private:
    std::condition_variable _condVar;
    std::mutex _mutex;
    std::queue<T> _queue;
};

}

#endif // COMMON_CONCURRENTQUEUE_H
