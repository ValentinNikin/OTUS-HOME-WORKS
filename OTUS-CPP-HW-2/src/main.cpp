#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>

#include "utils.h"
#include "ip.h"

// #define MEASURE_EXECUTION_TIME

template<typename T>
void print(const std::vector<T>& arr, const std::function<bool(const T&)> pred = nullptr) {
    for (const auto& item : arr) {
        if (pred == nullptr) {
            std::cout << item << std::endl;
        }
        else if (pred(item)) {
            std::cout << item << std::endl;
        }
    }
}

int main (int, char **) {

    #ifdef MEASURE_EXECUTION_TIME
        auto startTimePoint = std::chrono::high_resolution_clock::now();
    #endif

    std::vector<IpAddress> addresses;
    addresses.reserve(100);

    try {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                std::cout << "Error. Input line is empty" << std::endl;
                return 1;
            }

            auto tabIndex = line.find('\t');
            if (tabIndex == std::string::npos) {
                std::cout << "Error. Wrong line format. Unable to find '\\t'." << std::endl;
                return 1;
            }

            line = line.substr(0, tabIndex);

            addresses.emplace_back(IpAddress::parse(line));
        }
    }
    catch (const std::exception& ex) {
        std::cout << "Error. Problem occured while input parsing, what: " << ex.what() << std::endl;
        return 1;
    }

    std::sort(
        addresses.begin(), 
        addresses.end(), 
        [](const IpAddress& addr1, const IpAddress& addr2) { return addr2 < addr1; });

    print(addresses);
    print<IpAddress>(addresses, [](const IpAddress& addr) { return addr.byte1() == 1; });
    print<IpAddress>(addresses, [](const IpAddress& addr) { return addr.byte1() == 46 && addr.byte2() == 70; });
    print<IpAddress>(addresses, [](const IpAddress& addr) { return addr.byte1() == 46 || addr.byte2() == 46 || addr.byte3() == 46 || addr.byte4() == 46; });

    #ifdef MEASURE_EXECUTION_TIME
        auto endTimePoint = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTimePoint - startTimePoint);
        std::cout << "Execution time (ms): " << duration.count() << std::endl;
    #endif

    return 0;
}
