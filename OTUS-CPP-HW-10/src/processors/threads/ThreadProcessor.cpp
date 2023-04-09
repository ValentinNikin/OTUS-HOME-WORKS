#include "ThreadProcessor.h"

#include <iostream>

ThreadProcessor::ThreadProcessor(const std::string& name)
    : _name(name) {}

ThreadProcessor::~ThreadProcessor() {
    stop();
}

void ThreadProcessor::threadFunc() {
    while (_okToContinue || !_queue->isEmpty()) {
        auto block = _queue->pop();
        if (block != nullptr) {
            processBlock(block);
        }
        else {
            std::unique_lock<std::mutex> ul(_mutex);
            if (_okToContinue) {
                _cv.wait(ul, [this] { return !_queue->isEmpty() || !_okToContinue; });
            }
        }
    }
}

void ThreadProcessor::setQueue(ThreadSafeQueue* queue) {
    _queue = queue;
    _queue->setHaveNewItemsCallback([this]() {
        std::lock_guard<std::mutex> lg(_mutex);
        _cv.notify_one();
    });
}

void ThreadProcessor::run() {
    if (_queue == nullptr) {
        throw std::runtime_error("Unable to start thread while queue is unset");
    }
    _thread = std::thread(&ThreadProcessor::threadFunc, this);
}

void ThreadProcessor::stop() {
    {
        std::lock_guard<std::mutex> lg(_mutex);
        _okToContinue = false;
        _cv.notify_one();
    }
    if (_thread.joinable()) {
        _thread.join();
    }
}