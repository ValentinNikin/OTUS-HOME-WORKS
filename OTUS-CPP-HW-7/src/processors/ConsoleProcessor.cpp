#include "ConsoleProcessor.h"

#include <iostream>

void ConsoleProcessor::processCommands(std::shared_ptr<Block> block) {
    std::cout << "bulk: ";
    for (const auto& c : block->getCommands()) {
        std::cout << c << "; ";
    }
    std::cout << std::endl;
}