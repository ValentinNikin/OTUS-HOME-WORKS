#include "FileThreadProcessor.h"

#include <fstream>
#include <iostream>

#include "utils/Utils.h"

FileThreadProcessor::FileThreadProcessor(const std::string& name)
    : ThreadProcessor(name) {}

FileThreadProcessor::~FileThreadProcessor() {
    stop();
}

void FileThreadProcessor::processBlock(const std::shared_ptr<Block>& block) {
    auto currentTimePoint = Utils::getCurrentTimeInMilliseconds();

    auto fileName = "bulk" + std::to_string(block->getFillingStartTime()) + "_" + std::to_string(currentTimePoint) + "_" + _name + ".log";
    std::fstream stream(fileName, std::ios_base::out | std::ios_base::trunc);
    stream << "bulk: ";
    for (const auto& c : block->getCommands()) {
        stream << c << "; ";
    }
    stream << std::endl;

    // without this blocking, files can be overwritten
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}