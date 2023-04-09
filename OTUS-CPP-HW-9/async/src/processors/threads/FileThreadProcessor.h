#pragma once

#include "ThreadProcessor.h"

class FileThreadProcessor : public ThreadProcessor {
public:
    FileThreadProcessor(const std::string& name);
    ~FileThreadProcessor() override;
protected:
    void processBlock(const std::shared_ptr<Block>& block) override;
};