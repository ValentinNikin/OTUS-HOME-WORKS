#pragma once

#include "ThreadProcessor.h"

class ConsoleThreadProcessor : public ThreadProcessor {
public:
    ConsoleThreadProcessor(const std::string& name);
    ~ConsoleThreadProcessor() override;
protected:
    void processBlock(const std::shared_ptr<Block>& block) override;
};