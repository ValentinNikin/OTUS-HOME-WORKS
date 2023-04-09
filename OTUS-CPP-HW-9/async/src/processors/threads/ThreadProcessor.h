#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>

#include "processors/ThreadSafeQueue.h"

class ThreadProcessor {
public:
    ThreadProcessor(const std::string& name);
    virtual ~ThreadProcessor();

    void setQueue(ThreadSafeQueue* queue);

    void run();
    void stop();
protected:
    virtual void processBlock(const std::shared_ptr<Block>& block) = 0;

    std::string _name;
private:
    void threadFunc();
    std::thread _thread;

    std::mutex _mutex;
    std::condition_variable _cv;

    ThreadSafeQueue* _queue {nullptr};
    std::atomic_bool _okToContinue {true};
};
