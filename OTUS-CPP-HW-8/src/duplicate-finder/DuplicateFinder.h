#pragma once

#include <vector>
#include <string>
#include <list>

#include <boost/filesystem/path.hpp>

#include "types/FileInfo.h"
#include "hash/HashCalculator.h"
#include "FileComparator.h"

class DuplicateFinder {
public:
    DuplicateFinder(const std::shared_ptr<HashCalculator>& hashCalculator, std::size_t blockSize);

    std::vector<std::vector<boost::filesystem::path>> findDuplicates(const std::vector<FileInfo>& files);

private:
    std::shared_ptr<HashCalculator> _hashCalculator;
    std::size_t _blockSize;

    std::unique_ptr<FileComparator> _fileComparator;
};