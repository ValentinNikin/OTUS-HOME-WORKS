#include "FileLinesReader.h"

FileLinesReader::FileLinesReader(const std::string& filePath)
        : _filePath(filePath) {}

void FileLinesReader::setRanges(const std::size_t left, const std::size_t right) {
    if (left >= right) {
        throw std::runtime_error("Left point should be less than right");
    }

    _left = left;
    _right = right;
    _rangesIsSet = true;
}

void FileLinesReader::open() {
    _stream.open(_filePath, std::ios_base::in);
    if (!isOpen()) {
        throw std::runtime_error("Unable to open file " + _filePath);
    }

    _stream.seekg(0, std::ios_base::end);
    _length = static_cast<std::size_t>(_stream.tellg());
    _stream.seekg(0, std::ios_base::beg);

    if (_rangesIsSet) {
        if (_right > _length) {
            throw std::runtime_error("Right point should be less or equal to file length");
        }
        _stream.seekg(_left, std::ios_base::beg);
    }
}

bool FileLinesReader::isOpen() const {
    return _stream.is_open();
}

std::string FileLinesReader::getLastReadLine() const {
    return _lastReadLine;
}

bool FileLinesReader::readNextLine() {
    if (!(
            _stream.eof() ||
            (_rangesIsSet && static_cast<std::size_t>(_stream.tellg()) >= _right)
    )) {
        std::getline(_stream, _lastReadLine);
        _lastGCount = _stream.gcount();
        if (_lastReadLine.empty()) {
            return readNextLine();
        }
        return true;
    }
    _lastReadLine = "";
    _lastGCount = 0;
    return false;
}

std::size_t FileLinesReader::tellg() {
    return static_cast<std::size_t>(_stream.tellg());
}

bool FileLinesReader::isEof() const {
    return _stream.eof();
}

std::size_t FileLinesReader::getLength() const {
    return _length;
}

std::size_t FileLinesReader::getLastGCount() const {
    return _lastGCount;
}