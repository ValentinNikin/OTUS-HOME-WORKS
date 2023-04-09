#include "ReducerTask.h"

#include "FileLinesReader.h"

ReducerTask::ReducerTask(
        const std::string& inFilePath,
        const std::string& outFilePath,
        std::unique_ptr<IReducer> reducer,
        std::unique_ptr<IMapper> mapper)
        : _inFilePath(inFilePath),
        _outFilePath(outFilePath),
        _reducer(std::move(reducer)),
        _mapper(std::move(mapper)) {}

namespace {

std::string threadFunc(
        const std::string& inFilePath, const std::string& outFilePath, IReducer* reducer, IMapper* mapper) {
    FileLinesReader reader(inFilePath);
    reader.open();

    auto result = reducer->makeInitialResult();

    while (!reader.isEof() && reader.readNextLine()) {
        auto mapRes = mapper->deserialize(reader.getLastReadLine());
        reducer->reduce(result.get(), mapRes.get());
    }

    std::ofstream outFile(outFilePath, std::ios_base::out);
    outFile << result->serialize() << std::endl;
    outFile.close();

    return outFilePath;
}

}

void ReducerTask::run() {
    _future = std::async(
            std::launch::async, threadFunc, _inFilePath, _outFilePath, _reducer.get(), _mapper.get());
}

std::string ReducerTask::getResult() {
    return _future.get();
}