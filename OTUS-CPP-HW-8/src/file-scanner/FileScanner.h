#pragma once

#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "types/FileInfo.h"

namespace fs = boost::filesystem;

class FileScanner {
public:
    FileScanner() = default;
    FileScanner(const std::vector<std::string>& scanDirs);

    void setScanDirs(const std::vector<std::string>& scanDirs);
    void setIgnoreDirs(const std::vector<std::string>& ignoreDirs);
    void setMinFileSize(std::size_t size);
    void setFilesMasks(const std::vector<std::string>& masks);
    void setIsRecursiveScan(bool isRecursiveScan);

    std::vector<FileInfo> scan();
private:

    using regex = boost::regex;

    std::vector<FileInfo> scanDirectory(const fs::path& dirPath);
    bool fileIsSatisfy(const FileInfo& fi) const;
    bool needIgnoreDirectory(const fs::path& dirPath) const;

    std::vector<fs::path> _scanDirs;
    std::vector<fs::path> _ignoreDirs;
    std::vector<regex> _filesMasks;
    std::size_t _minFileSize {1}; // in bytes
    bool _isRecursiveScan {false};
};