#include "FileBlockHashReader.h"

FileBlockHashReader::FileBlockHashReader(
        const FileInfo& fi, const std::shared_ptr<HashCalculator>& hashCalculator, std::size_t blockSize)
        : _fileInfo(fi), _hashCalculator(hashCalculator), _blockSize(blockSize) {
    if (_blockSize < 1) {
        throw std::runtime_error("Block size should be more than 1");
    }
}

FileBlockHashReader::~FileBlockHashReader() {
    if (_currentBlockData) {
        delete[] _currentBlockData;
    }
}

void FileBlockHashReader::open() {
    if (isOpen()) {
        _stream.close();
    }

    _stream.open(_fileInfo.path.c_str(), std::ios_base::in);
    if (!_stream.is_open()) {
        throw std::runtime_error("Unable to open file " + _fileInfo.path.string());
    }

    if (!_currentBlockData) {
        _currentBlockData = new uint8_t[_blockSize];
    }
    _currentBlockIdx = 0;
    _blocks.clear();
    _blocks.reserve(_fileInfo.size / _blockSize);
}

bool FileBlockHashReader::isOpen() const {
    return _stream.is_open();
}

bool FileBlockHashReader::readNextBlock(std::size_t& outBlockHash) {
    if (!isOpen()) {
        throw std::runtime_error("Before reading blocks you should call open() method");
    }

    if (_currentBlockIdx < _blocks.size()) {
        outBlockHash = _blocks[_currentBlockIdx];
        _currentBlockIdx++;
        return true;
    }

    if (_stream.eof()) return false;

    _stream.read((char*)_currentBlockData, _blockSize);

    auto readBytesCount = static_cast<std::size_t>(_stream.gcount());
    if (readBytesCount < _blockSize) {
        memset(_currentBlockData + readBytesCount, '\0', _blockSize - readBytesCount);
    }

    outBlockHash = _hashCalculator->calculateHash(_currentBlockData, _blockSize);

    _blocks.push_back(outBlockHash);
    _currentBlockIdx++;

    return true;
}

void FileBlockHashReader::reset() {
    _currentBlockIdx = 0;
}

std::size_t FileBlockHashReader::getSize() const {
    return _fileInfo.size;
}

boost::filesystem::path FileBlockHashReader::getFilePath() const {
    return _fileInfo.path;
}