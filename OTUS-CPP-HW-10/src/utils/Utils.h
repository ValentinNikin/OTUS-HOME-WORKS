#pragma once

#include <vector>
#include <string>

namespace Utils {
    std::vector<std::string> split(const std::string& str, const char delimiter);

    std::size_t getCurrentTimeInMilliseconds();
}