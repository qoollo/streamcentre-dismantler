#ifndef COMMON_SEMAPHORE_H
#define COMMON_SEMAPHORE_H

#include <mutex>
#include <condition_variable>


namespace vseg {

class Semaphore {
public:
    Semaphore();

    void notify();
    void wait();

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    unsigned int _count;
};

}

#endif // COMMON_SEMAPHORE_H
