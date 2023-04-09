#pragma once

#include <fstream>
#include <memory>

#include <boost/filesystem.hpp>

#include "types/FileInfo.h"
#include "hash/HashCalculator.h"

/**
 * Обертка над файлом для подсчета хэшей блоков
 */
class FileBlockHashReader {
public:
    FileBlockHashReader(const FileInfo& fi, const std::shared_ptr<HashCalculator>& hashCalculator, std::size_t blockSize);
    ~FileBlockHashReader();

    void open();
    bool isOpen() const;

    /**
     * Прочитать следующий блок файла
     * @param outBlockHash результирующий хэш блока
     * @return true - удалось прочитать блок, false - не удалось прочитать блок
     */
    bool readNextBlock(std::size_t& outBlockHash);

    /**
     * Вернуть индекс блока в начальное положение
     */
    void reset();

    std::size_t getSize() const;
    boost::filesystem::path getFilePath() const;
private:
    FileInfo _fileInfo;

    std::ifstream _stream;

    std::shared_ptr<HashCalculator> _hashCalculator;

    // хэши прочитанных блоков
    std::vector<std::size_t> _blocks;
    std::size_t _currentBlockIdx;
    uint8_t* _currentBlockData = nullptr;
    std::size_t _blockSize;
};
