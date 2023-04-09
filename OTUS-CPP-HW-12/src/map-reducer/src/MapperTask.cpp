#include "MapperTask.h"

#include <fstream>
#include <list>

#include "FileLinesReader.h"

MapperTask::MapperTask(
        const std::string& inFilePath, Range range, std::unique_ptr<IMapper> mapper)
    : _inFilePath(inFilePath), _range(range), _mapper(std::move(mapper)) {}

namespace {

std::string threadFunc(const std::string& inFilePath, const Range& range, IMapper* mapper) {
    FileLinesReader reader(inFilePath);
    reader.setRanges(range.left, range.right);
    reader.open();

    std::list<std::unique_ptr<IMapperResult>> results;

    while (!reader.isEof() && reader.readNextLine()) {
        auto mapResult = mapper->map(reader.getLastReadLine());

        // сразу выполняем сортировку, ищем место в списке результатов
        auto it = results.begin();
        while (it != results.cend()) {
            if (!(*it->get() < mapResult.get())) {
                break;
            }
            it++;
        }
        results.insert(it, std::move(mapResult));
    }

    std::string outFileName =
            "map_" + std::to_string(range.left) +
            "_" + std::to_string(range.right) +
            "_" + std::to_string(time(nullptr)) + ".txt";

    std::ofstream outFile(outFileName, std::ios_base::out);

    auto it = results.begin();
    while (it != results.end()) {
        outFile << it->get()->serialize() << std::endl;
        it++;
    }

    outFile.close();

    return outFileName;
}

}

void MapperTask::run() {
    _future = std::async(std::launch::async, threadFunc, _inFilePath, _range, _mapper.get());
}

std::string MapperTask::getResult() {
    return _future.get();
}