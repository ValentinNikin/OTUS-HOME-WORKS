#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <functional>

#include "types/Block.h"

class ThreadSafeQueue {
public:
    void push(const std::shared_ptr<Block>& block);
    std::shared_ptr<Block> pop();

    bool isEmpty() const;
    std::size_t size() const;

    void setHaveNewItemsCallback(const std::function<void()>& callback);
private:
    std::queue<std::shared_ptr<Block>> _queue;
    mutable std::mutex _mutex;

    std::vector<std::function<void()>> _haveNewItemsСallbaсks;
};