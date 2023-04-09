#pragma once

#include <vector>
#include <string>

struct Arguments {
    bool help {false};

    std::vector<std::string> scanDirs;
    std::vector<std::string> ignoreScanDirs;
    bool recursiveScan {false};
    std::size_t minFileSize {1};
    std::vector<std::string> filesMasks;
    std::size_t readerBlockSize {1};
    std::string hashFunc {"md5"};
};

