#include "MultiThreadProcessor.h"

MultiThreadProcessor::MultiThreadProcessor() {
    _queue = std::make_unique<ThreadSafeQueue>();
}

void MultiThreadProcessor::addThread(std::unique_ptr<ThreadProcessor> threadProcessor) {
    threadProcessor->setQueue(_queue.get());
    threadProcessor->run();
    _threadsProcessors.push_back(std::move(threadProcessor));
}

void MultiThreadProcessor::processBlock(std::shared_ptr<Block> block) {
    _queue->push(block);
}

void MultiThreadProcessor::stop() {
    for (const auto& tp : _threadsProcessors) {
        tp->stop();
    }
}