#include "Utils.h"

#include <sstream>
#include <chrono>

std::vector<std::string> Utils::split(const std::string& str, const char delimiter) {
    std::vector<std::string> arr;
    std::stringstream strStream(str);
    std::string segment;

    while (std::getline(strStream, segment, delimiter)) {
        arr.push_back(segment);
    }

    return arr;
}

std::size_t Utils::getCurrentTimeInMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}
