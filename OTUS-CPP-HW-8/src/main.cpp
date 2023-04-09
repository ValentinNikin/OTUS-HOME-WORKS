
#include <iostream>

#include "utils/ArgumentsReader.h"
#include "file-scanner/FileScanner.h"
#include "duplicate-finder/DuplicateFinder.h"
#include "duplicate-finder/hash/DefaultHashCalculator.h"
#include "duplicate-finder/hash/Crc32Calculator.h"

int main (int argc, char** argv) {

    // парсинг аргументов
    ArgumentsReader argumentsReader;
    Arguments arguments;
    try {
        arguments = argumentsReader.parse(argc, argv);
    }
    catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    if (arguments.help) {
        argumentsReader.printHelp();
        return 0;
    }

    std::vector<std::string> allowableHashFunctions = { "default", "crc32" };
    if (std::find(allowableHashFunctions.cbegin(), allowableHashFunctions.cend(), arguments.hashFunc) == allowableHashFunctions.cend()) {
        std::cerr << "Unsupported hash function: \"" << arguments.hashFunc << "\"" << std::endl;
        return 1;
    }

    // получаем список файлов согласно заданным фильтрам
    FileScanner fileScanner(arguments.scanDirs);
    fileScanner.setIgnoreDirs(arguments.ignoreScanDirs);
    fileScanner.setFilesMasks(arguments.filesMasks);
    fileScanner.setMinFileSize(arguments.minFileSize);
    fileScanner.setIsRecursiveScan(arguments.recursiveScan);

    auto files = fileScanner.scan();

    std::shared_ptr<HashCalculator> hashCalculator;
    if (arguments.hashFunc == "default") {
        hashCalculator = std::make_shared<DefaultHashCalculator>();
    }
    else if (arguments.hashFunc == "crc32") {
        hashCalculator = std::make_shared<Crc32Calculator>();
    }

    // выполняем поиск дубликатов
    DuplicateFinder duplicateFinder(hashCalculator, arguments.readerBlockSize);
    auto duplicatesGroups = duplicateFinder.findDuplicates(files);

    for (const auto& group : duplicatesGroups) {
        for (const auto& duplicate : group) {
            std::cout << duplicate << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
