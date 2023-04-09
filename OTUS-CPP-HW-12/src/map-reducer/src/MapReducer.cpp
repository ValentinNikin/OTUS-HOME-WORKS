#include "map-reducer/include/MapReducer.h"

#include <vector>
#include <algorithm>
#include <cstdio>

#include "MapperTask.h"
#include "ReducerTask.h"
#include "Splitter.h"
#include "Shuffler.h"

MapReducer::MapReducer(const std::size_t mThreads, const std::size_t rThreads)
    : _mThreads(mThreads), _rThreads(rThreads) {}

void MapReducer::setMapperGenerator(const MapperGenerator& func) {
    _mapperGenerator = func;
}

void MapReducer::setReducerGenerator(const ReducerGenerator& func) {
    _reducerGenerator = func;
}

std::vector<std::string> MapReducer::map(
        const std::string& inFilePath, const std::vector<Range>& ranges) {

    std::vector<MapperTask> mapperTasks;
    std::transform(ranges.begin(), ranges.end(), std::back_inserter(mapperTasks),
                   [&inFilePath, mg = _mapperGenerator](const auto& range) {
                        MapperTask mt(inFilePath, range, mg());
                        mt.run();
                        return mt;
                    });

    std::vector<std::string> results;
    std::transform(
            mapperTasks.begin(), mapperTasks.end(), std::back_inserter(results),
            [](auto& mt) {
                return mt.getResult();
            });

    return results;
}

std::vector<std::string> MapReducer::reduce(
        const std::vector<std::string>& inFilesPaths, const std::string& outFilesPath) {
    std::vector<ReducerTask> reducerTasks;
    std::size_t index = 0;
    std::transform(inFilesPaths.cbegin(), inFilesPaths.cend(), std::back_inserter(reducerTasks),
                   [rg = _reducerGenerator, mg = _mapperGenerator, &index, &outFilesPath](const std::string& filePath) {
        auto outFilePath = outFilesPath + "/reduce_" + std::to_string(index) + ".txt";
        index++;

        ReducerTask reducerTask(filePath, outFilePath, rg(), mg());
        reducerTask.run();
        return reducerTask;
    });

    std::vector<std::string> results;
    std::transform(
            reducerTasks.begin(), reducerTasks.end(), std::back_inserter(results),
            [](auto& mt) {
                return mt.getResult();
            });

    return results;
}

namespace {

void removeFiles(const std::vector<std::string>& files) {
    for (std::size_t i = 0; i < files.size(); i++) {
        std::remove(files[i].c_str());
    }
}

}

std::vector<std::string> MapReducer::run(
        const std::string& inFilePath, const std::string& outFilesPath) {
    if (_mapperGenerator == nullptr || _reducerGenerator == nullptr) {
        throw std::runtime_error("You should specify generators for mapper and reducer");
    }

    // Разбиваем файл на необходимое количество потоков
    Splitter splitter(inFilePath);
    auto ranges = splitter.split(_mThreads);

    // Этап "map"
    auto mapperStageFiles = map(inFilePath, ranges);

    // Разбиваем полученные в результате этапа "map" файлы на необходимое количество потоков.
    Shuffler shuffler(_mapperGenerator());
    auto filesForReducer = shuffler.process(mapperStageFiles, _rThreads);

    // Этап "reduce"
    auto reducerStageFiles = reduce(filesForReducer, outFilesPath);

    // Удаляем временные файлы
    removeFiles(mapperStageFiles);
    removeFiles(filesForReducer);

    return reducerStageFiles;
}