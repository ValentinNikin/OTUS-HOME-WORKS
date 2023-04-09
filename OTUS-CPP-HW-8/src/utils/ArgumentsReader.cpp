#include "ArgumentsReader.h"

#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

constexpr int64_t mb10 = 1024 * 1024 * 10; // 10 Mb

namespace {
    void inRange(int64_t min, int64_t max, uint32_t value, char const * const optName) {
        if (value < min || value > max) {
            throw po::validation_error(po::validation_error::invalid_option_value,
                                       optName, std::to_string(value));
        }
    }

}

ArgumentsReader::ArgumentsReader()
    : _desc(boost::program_options::options_description("Allowed options")) {

    _desc.add_options()
        ("help", "Produce help message")
        ("scan-dirs", po::value<std::vector<std::string>>()->required(), "List of directories to search duplicate files")
        ("ignore-scan-dirs", po::value<std::vector<std::string>>(), "List of directories to ignore in the process of searching")
        ("recursive-scan", "Recursive searching")
        ("min-file-size", po::value<uint32_t>()->default_value(1)->notifier(boost::bind(&inRange, 1, mb10, boost::placeholders::_1, "min-file-size")), "Minimal allowed file size to process (in bytes). 1 byte by default")
        ("files-masks", po::value<std::vector<std::string>>(), "Files names masks to process")
        ("block-size", po::value<uint32_t>()->default_value(1)->notifier(boost::bind(&inRange, 1, mb10, boost::placeholders::_1, "block-size")), "Block size to read files (in bytes). 1 byte by default")
        ("hash-func", po::value<std::string>()->default_value("default"), "Hash function to compare files (default or crc32)")
    ;
}

void ArgumentsReader::printHelp() const {
    std::cout << _desc << std::endl;
}

Arguments ArgumentsReader::parse(int argc, char** argv) {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, _desc), vm);
    po::notify(vm);

    Arguments args;

    if (vm.count("help")) {
        args.help = true;
        return args;
    }

    if (vm.count("scan-dirs")) {
        args.scanDirs = vm["scan-dirs"].as<std::vector<std::string>>();
    }

    if (vm.count("ignore-scan-dirs")) {
        args.ignoreScanDirs = vm["ignore-scan-dirs"].as<std::vector<std::string>>();
    }

    if (vm.count("recursive-scan")) {
        args.recursiveScan = true;
    }

    if (vm.count("min-file-size")) {
        args.minFileSize = static_cast<std::size_t>(vm["min-file-size"].as<uint32_t>());
    }

    if (vm.count("files-masks")) {
        args.filesMasks = vm["files-masks"].as<std::vector<std::string>>();
    }

    if (vm.count("block-size")) {
        args.readerBlockSize = static_cast<std::size_t>(vm["block-size"].as<uint32_t>());
    }

    if (vm.count("hash-func")) {
        args.hashFunc = vm["hash-func"].as<std::string>();
    }

    return args;
}