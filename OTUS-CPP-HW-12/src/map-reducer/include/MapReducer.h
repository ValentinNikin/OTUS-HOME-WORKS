#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <string>

#include "map-reducer/include/IMapper.h"
#include "map-reducer/include/IReducer.h"
#include "map-reducer/src/Range.h"

class MapReducer {
    using MapperGenerator = std::function<std::unique_ptr<IMapper>()>;
    using ReducerGenerator = std::function<std::unique_ptr<IReducer>()>;
public:
    MapReducer(const std::size_t mThreads, const std::size_t rThreads);

    void setMapperGenerator(const MapperGenerator& func);
    void setReducerGenerator(const ReducerGenerator& func);

    std::vector<std::string> run(const std::string& inFilePath, const std::string& outFilesPath);

private:
    std::size_t _mThreads;
    std::size_t _rThreads;
private:
    std::vector<std::string> map(const std::string& inFilePath, const std::vector<Range>& ranges);
    std::vector<std::string> shuffle(const std::vector<std::string>& filesPaths, const std::size_t desiredFilesCount);
    std::vector<std::string> reduce(const std::vector<std::string>& inFilesPaths, const std::string& outFilesPath);
private:
    MapperGenerator _mapperGenerator;
    ReducerGenerator _reducerGenerator;
};