#include "ThreadSafeQueue.h"

void ThreadSafeQueue::push(const std::shared_ptr<Block>& block) {
    {
        std::lock_guard<std::mutex> lg(_mutex);
        _queue.push(block);
    }

    std::for_each(
            _haveNewItemsСallbaсks.cbegin(), _haveNewItemsСallbaсks.cend(),
            [](const auto& c) { c(); });
}

std::shared_ptr<Block> ThreadSafeQueue::pop() {
    std::shared_ptr<Block> retBlock;
    {
        std::lock_guard<std::mutex> lg(_mutex);
        if (!_queue.empty()) {
            retBlock = _queue.front();
            _queue.pop();
        }
    }
    return retBlock;
}

bool ThreadSafeQueue::isEmpty() const {
    std::lock_guard<std::mutex> lg(_mutex);
    return _queue.empty();
}

std::size_t ThreadSafeQueue::size() const {
    std::lock_guard<std::mutex> lg(_mutex);
    return _queue.size();
}

void ThreadSafeQueue::setHaveNewItemsCallback(const std::function<void()>& callback) {
    _haveNewItemsСallbaсks.push_back(callback);
}