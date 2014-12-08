#include "Semaphore.h"

namespace vseg {

Semaphore::Semaphore()
    : _count()
{}

void Semaphore::notify() {
    std::unique_lock<std::mutex> lock(_mutex);
    ++ _count;
    _condition.notify_one();
}

void Semaphore::wait() {
    std::unique_lock<std::mutex> lock(_mutex);

    while(!_count)
        _condition.wait(lock);

    -- _count;
}

}
