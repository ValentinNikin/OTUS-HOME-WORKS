#pragma once

#include <boost/filesystem.hpp>

struct FileInfo {
    boost::filesystem::path path;
    std::size_t size;
};

struct FileInfoComparator
{
    inline bool operator() (const FileInfo& fi1, const FileInfo& fi2)
    {
        return (fi1.path < fi2.path);
    }
};