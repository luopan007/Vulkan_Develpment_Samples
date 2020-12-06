#include <mutex>
#include <condition_variable>
#ifndef SAMPLE17_8_SEMAPHORE_H
#define SAMPLE17_8_SEMAPHORE_H
class Semaphore {
public:
    Semaphore(int count);
    ~Semaphore();
    void WaitForSingleObject();
    void ReleaseSemaphore();
private:
    int count;
    std::mutex mutex;
    std::condition_variable condition;
};


#endif //SAMPLE17_8_SEMAPHORE_H
