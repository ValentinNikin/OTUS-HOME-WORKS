#pragma once

#include <string>
#include <future>

#include "Range.h"
#include "map-reducer/include/IMapper.h"

class MapperTask {
public:
    MapperTask(const std::string& inFilePath, Range range, std::unique_ptr<IMapper> mapper);

    void run();
    std::string getResult();
private:
    std::string _inFilePath;
    Range _range;
    std::unique_ptr<IMapper> _mapper;

    std::future<std::string> _future;
};