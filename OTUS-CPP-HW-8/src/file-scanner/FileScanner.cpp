#include "FileScanner.h"

#include <boost/algorithm/string.hpp>

namespace ba = boost::algorithm;

/**
 * filesystem examples
 * https://www.boost.org/doc/libs/1_74_0/libs/filesystem/doc/tutorial.html
 */

FileScanner::FileScanner(const std::vector<std::string>& scanDirs) {
    setScanDirs(scanDirs);
}

void FileScanner::setScanDirs(const std::vector<std::string>& scanDirs) {
    _scanDirs.reserve(scanDirs.size());
    for (const auto& dir : scanDirs) {
        auto absolutePath = fs::canonical(fs::absolute(dir));

        if (!fs::exists(absolutePath)) {
            throw std::runtime_error("Directory \"" + absolutePath.string() + "\" is not exist");
        }

        if (!fs::is_directory(absolutePath)) {
            throw std::runtime_error("\"" + absolutePath.string() + "\" is not directory");
        }

        _scanDirs.push_back(absolutePath);
    }
}

void FileScanner::setIgnoreDirs(const std::vector<std::string>& ignoreDirs) {
    _ignoreDirs.reserve(ignoreDirs.size());
    for (const auto& dir : ignoreDirs) {
        auto absolutePath = fs::canonical(fs::absolute(dir));

        if (std::find(_scanDirs.cbegin(), _scanDirs.cend(), absolutePath) != _scanDirs.cend()) {
            throw std::runtime_error("Items from \"scan-dirs\" list should not appear in the \"ignore-scan-dirs\" list");
        }

        if (fs::exists(absolutePath) && fs::is_directory(absolutePath)) {
            _ignoreDirs.push_back(absolutePath);
        }
    }
}

void FileScanner::setMinFileSize(std::size_t size) {
    _minFileSize = size;
}

namespace {
    std::string prepareMask(const std::string& inMask) {
        auto preparedMask = ba::to_lower_copy(inMask);
        ba::replace_all(preparedMask, ".", "\\.");
        ba::replace_all(preparedMask, "?", ".?");
        ba::replace_all(preparedMask, "*", ".*");

        return preparedMask;
    }
}

void FileScanner::setFilesMasks(const std::vector<std::string>& masks) {
    for (const auto& m : masks) {
        _filesMasks.push_back(regex(prepareMask(m)));
    }
}

void FileScanner::setIsRecursiveScan(bool isRecursiveScan) {
    _isRecursiveScan = isRecursiveScan;
}

namespace {
    template<typename T>
    void mergeVectors(std::vector<T>& first, std::vector<T>&& second) {
        first.reserve(first.size() + second.size());
        first.insert(first.end(), second.begin(), second.end());
    }
}

std::vector<FileInfo> FileScanner::scan() {
    if (_scanDirs.empty()) {
        throw std::runtime_error("Directories for scanning not specified");
    }

    std::vector<FileInfo> files;

    for (const auto& dir : _scanDirs) {
        mergeVectors(files, scanDirectory(dir));
    }

    return files;
}

std::vector<FileInfo> FileScanner::scanDirectory(const fs::path& dirPath) {
    std::vector<FileInfo> files;

    for (const fs::directory_entry& e : fs::directory_iterator(dirPath)) {
        auto path = e.path();
        if (fs::is_regular(path)) {
            FileInfo fi {path, static_cast<std::size_t>(fs::file_size(e))};
            if (fileIsSatisfy(fi)) {
                files.push_back(fi);
            }
        }
        else if (fs::is_directory(path) && _isRecursiveScan && !needIgnoreDirectory(path)) {
            mergeVectors(files, scanDirectory(path));
        }
    }

    return files;
}

bool FileScanner::fileIsSatisfy(const FileInfo& fi) const {
    if (fi.size < _minFileSize) return false;
    if (_filesMasks.empty()) return true;

    return std::any_of(
            _filesMasks.cbegin(),
            _filesMasks.cend(),
            [filename = ba::to_lower_copy(fi.path.filename().string())](const regex& filter) {
        boost::smatch what;
        return boost::regex_match(filename, what, filter);
    });
}

bool FileScanner::needIgnoreDirectory(const fs::path& dirPath) const {
    return std::find(_ignoreDirs.cbegin(), _ignoreDirs.cend(), dirPath) != _ignoreDirs.cend();
}