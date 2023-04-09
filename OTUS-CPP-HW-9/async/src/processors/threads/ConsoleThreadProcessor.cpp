#include "ConsoleThreadProcessor.h"

#include <iostream>

ConsoleThreadProcessor::ConsoleThreadProcessor(const std::string& name)
    : ThreadProcessor(name) {}

ConsoleThreadProcessor::~ConsoleThreadProcessor() {
    stop();
}

void ConsoleThreadProcessor::processBlock(const std::shared_ptr<Block>& block) {
    std::cout << "bulk [" << _name << "]: ";
    for (const auto& c : block->getCommands()) {
        std::cout << c << "; ";
    }
    std::cout << std::endl;
}