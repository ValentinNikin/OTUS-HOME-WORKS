#include "Shuffler.h"

#include <fstream>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "FileLinesReader.h"

Shuffler::Shuffler(std::unique_ptr<IMapper> mapper)
    : _mapper(std::move(mapper)) {}

namespace {

class ShufflerFileWrapper {
public:
    ShufflerFileWrapper(const std::string& filePath, IMapper* mapper)
            : _reader(FileLinesReader(filePath)),
              _mapper(mapper) {
        open();
    }

    void open() {
        _reader.open();
        readNextObject();
    }

    bool isEof() const {
        return _isEof;
    }

    IMapperResult* getTopObject() {
        if (isEof()) {
            return nullptr;
        }

        return _topObj.get();
    }

    std::unique_ptr<IMapperResult> takeTopObject() {
        if (isEof()) {
            return nullptr;
        }

        auto currentObject = std::move(_topObj);
        readNextObject();

        return currentObject;
    }

    std::size_t getLength() const {
        return _reader.getLength();
    }

    std::size_t getLastGCount() const {
        return _reader.getLastGCount();
    }

private:
    void readNextObject() {
        if (!_reader.readNextLine()) {
            _isEof = true;
        }
        else {
            _topObj = _mapper->deserialize(_reader.getLastReadLine());
        }
    }
private:
    FileLinesReader _reader;
    IMapper* _mapper;

    bool _isEof {false};
    std::unique_ptr<IMapperResult> _topObj;
};

bool isEof(const std::vector<ShufflerFileWrapper>& sfWrappers) {
    return std::all_of(
            sfWrappers.cbegin(),
            sfWrappers.cend(),
            [](const auto& sf) { return sf.isEof(); });
}

std::size_t findMinObject(std::vector<ShufflerFileWrapper>& sfWrappers) {
    if (isEof(sfWrappers)) {
        return SIZE_MAX;
    }

    std::size_t i;
    for (i = 0; i < sfWrappers.size(); i++) {
        if (!sfWrappers[i].isEof()) {
            break;
        }
    }
    std::size_t minObjIdx = i;

    for (i = i + 1; i < sfWrappers.size(); i++) {
        if (!sfWrappers[i].isEof() && sfWrappers[i].getTopObject()->less(sfWrappers[minObjIdx].getTopObject())) {
            minObjIdx = i;
        }
    }

    return minObjIdx;
}

}

std::vector<std::string> Shuffler::process(
        const std::vector<std::string>& filesPaths, const std::size_t desiredFilesCount) {
    std::vector<ShufflerFileWrapper> sfWrappers;
    std::transform(
            filesPaths.cbegin(), filesPaths.cend(), std::back_inserter(sfWrappers),
            [mapper = _mapper.get()](const auto& file) {
                return ShufflerFileWrapper(file, mapper);
            });

    std::size_t fullDataLength = std::accumulate(
            sfWrappers.cbegin(), sfWrappers.cend(), static_cast<std::size_t>(0),
            [](std::size_t res, const auto& sf) {
                return res + sf.getLength();
            });

    std::vector<std::string> outputFilesPaths;

    // целевой размер файла для следующего reduce-шага
    auto outputTargetFileSize = fullDataLength / desiredFilesCount;

    std::size_t currentFileIdx = 0;
    std::string outFileName =
            "range_in_" + std::to_string(currentFileIdx) +
            "_" + std::to_string(time(nullptr)) + ".txt";
    std::ofstream outFile(outFileName, std::ios_base::out);
    bool needOpenNextFile = false;
    bool objectsSeriesClosed = false;
    std::unique_ptr<IMapperResult> prevObject;

    /*
     * Процедура смешивания данных.
     * Содержимое файлов отсортировано.
     * Каждый из файлов представляем в виде своеобразного стека (обертка ShufflerFileWrapper).
     * 1. Берем минимальный из элементов, находящихся на вершине стека.
     * 2. Добавляем этот элемент в выходной файл.
     * 3. Когда новый файл достаточно заполнен, и серия объектов с заданным ключом завершена,
     * открываем новый файл и продолжаем обработку.
     */

    while (!isEof(sfWrappers)) {
        auto minObjIdx = findMinObject(sfWrappers);
        if (minObjIdx == SIZE_MAX) {
            throw std::runtime_error("Min object not found");
        }

        auto currentObject = sfWrappers[minObjIdx].takeTopObject();

        objectsSeriesClosed = prevObject != nullptr && !currentObject->equal(prevObject.get());
        needOpenNextFile = static_cast<std::size_t>(outFile.tellp()) > outputTargetFileSize;

        if (objectsSeriesClosed && needOpenNextFile) {
            outFile.close();
            outputFilesPaths.push_back(outFileName);

            currentFileIdx++;
            outFileName = "range_in_" + std::to_string(currentFileIdx) +
                          "_" + std::to_string(time(nullptr)) + ".txt";
            outFile.open(outFileName, std::ios_base::out);
        }

        outFile << currentObject->serialize() << std::endl;
        prevObject = std::move(currentObject);
    }

    outFile.close();
    outputFilesPaths.push_back(outFileName);

    return outputFilesPaths;
}