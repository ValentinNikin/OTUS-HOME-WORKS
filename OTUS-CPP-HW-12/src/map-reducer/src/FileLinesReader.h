#pragma once

#include <string>
#include <fstream>

class FileLinesReader {
public:
    FileLinesReader(const std::string& filePath);

    void setRanges(const std::size_t left, const std::size_t right);
    void open();
    bool isOpen() const;

    bool readNextLine();

    std::size_t tellg();
    bool isEof() const;

    std::string getLastReadLine() const;
    std::size_t getLength() const;
    std::size_t getLastGCount() const;

private:
    std::ifstream _stream;

    std::string _lastReadLine;
    std::size_t _lastGCount {0};

    bool _rangesIsSet {false};
    std::size_t _left;
    std::size_t _right;

    std::size_t _length;

    const std::string _filePath;
};