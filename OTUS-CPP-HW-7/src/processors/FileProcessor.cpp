#include "FileProcessor.h"

#include <fstream>

void FileProcessor::processCommands(std::shared_ptr<Block> block) {
    auto fileName = "bulk" + std::to_string(block->getFillingStartTime()) + ".log";
    std::fstream stream(fileName, std::ios_base::out | std::ios_base::trunc);
    stream << "bulk: ";
    for (const auto& c : block->getCommands()) {
        stream << c << "; ";
    }
    stream << std::endl;
}