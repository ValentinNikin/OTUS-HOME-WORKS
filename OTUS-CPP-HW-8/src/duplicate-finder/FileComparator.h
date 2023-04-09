#pragma once

#include "./FileBlockHashReader.h"

class FileComparator {
public:
    bool compare(FileBlockHashReader* file1, FileBlockHashReader* file2);
};