#pragma once

#include <vector>
#include <string>

#include "map-reducer/include/IMapper.h"

class Shuffler {
public:
    Shuffler(std::unique_ptr<IMapper> mapper);

    std::vector<std::string> process(const std::vector<std::string>& filesPaths, const std::size_t desiredFilesCount);
private:
    std::unique_ptr<IMapper> _mapper;
};
