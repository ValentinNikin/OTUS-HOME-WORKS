#pragma once

#include <memory>

#include "CommandBundler.h"

class CommandReader {
public:
    CommandReader(std::unique_ptr<CommandBundler> bundler);

    void run();
private:
    std::unique_ptr<CommandBundler> _bundler;
};
