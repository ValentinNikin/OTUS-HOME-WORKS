#include "Splitter.h"

#include <fstream>
#include <array>
#include <algorithm>

Splitter::Splitter(const std::string& filePath)
    : _filePath(filePath) {}

std::vector<Range> Splitter::split(const std::size_t desiredBlocks) {
    std::ifstream file(_filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file " + _filePath + " for reading");
    }

    file.seekg(0, std::ios_base::end);
    const std::size_t length = static_cast<std::size_t>(file.tellg());
    file.seekg(0, std::ios_base::beg);

    std::vector<Range> ranges;
    auto blockSize = length / desiredBlocks;

    std::size_t left = 0;
    std::size_t right = 0;
    std::size_t gcount = 0;

    constexpr std::size_t bufSize = 50;
    std::array<char, bufSize> buffer;

    while (right < length) {
        left = right;
        right = std::min(right + blockSize, length);

        file.seekg(right, std::ios_base::beg);

        while (right < length) {
            file.read(&buffer[0], bufSize);
            gcount = file.gcount();
            if (file.eof()) { // reset failbit after file is eof
                file.clear();
            }
            auto it = std::find(buffer.cbegin(), buffer.cbegin() + gcount, '\n');
            if (it != buffer.cend()) {
                right += std::distance(buffer.cbegin(), it) + 1;
                break;
            }
            right += gcount;
        }

        ranges.emplace_back(left, right);
    }

    return ranges;
}