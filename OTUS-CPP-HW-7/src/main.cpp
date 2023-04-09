
#include <iostream>

#include "CommandBundler.h"
#include "CommandReader.h"
#include "processors/ConsoleProcessor.h"
#include "processors/FileProcessor.h"

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "You should specify block size argument value" << std::endl;
        std::cerr << "For example: " << argv[0] << " 3" << std::endl;
        return 1;
    }

    auto blockSize = atoi(argv[1]);

    auto commandBundler = std::make_unique<CommandBundler>(blockSize);
    commandBundler->addProcessor(std::make_shared<ConsoleProcessor>());
    commandBundler->addProcessor(std::make_shared<FileProcessor>());

    auto commandReader = std::make_unique<CommandReader>(std::move(commandBundler));
    commandReader->run();

    return 0;
}
