#pragma once

#include <string>
#include <vector>

#include "Range.h"

class Splitter {
public:
    Splitter(const std::string& filePath);
    std::vector<Range> split(const std::size_t desiredBlocks);
private:
    std::string _filePath;
};
