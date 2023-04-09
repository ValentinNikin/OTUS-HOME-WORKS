#pragma once

#include "IBlockProcessor.h"
#include "ThreadSafeQueue.h"
#include "threads/ThreadProcessor.h"

class MultiThreadProcessor : public IBlockProcessor {
public:
    MultiThreadProcessor();

    void addThread(std::unique_ptr<ThreadProcessor> threadProcessor);

    void processBlock(std::shared_ptr<Block> block) override;

    void stop();
protected:
    /// Очередь блоков
    std::unique_ptr<ThreadSafeQueue> _queue;
    std::vector<std::unique_ptr<ThreadProcessor>> _threadsProcessors;
};