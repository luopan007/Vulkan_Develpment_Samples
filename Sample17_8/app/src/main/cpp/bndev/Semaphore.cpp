#include "Semaphore.h"
Semaphore::Semaphore(int count)
{
    this->count = count;
}
Semaphore::~Semaphore()
{
}
//消耗资源，P方法
void Semaphore::WaitForSingleObject() {
    std::unique_lock<std::mutex> lock{ mutex };
    count--;
    if (count < 0) {
        condition.wait(lock);
    }
}
//释放资源，V方法
void Semaphore::ReleaseSemaphore() {
    std::lock_guard<std::mutex> lock{ mutex };
    count++;
    if (count <= 0) {
        condition.notify_one();
    }
}
