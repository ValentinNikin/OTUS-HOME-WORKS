#include "App.h"

#include <iostream>

#include "processors/MultiThreadProcessor.h"
#include "processors/threads/ConsoleThreadProcessor.h"
#include "processors/threads/FileThreadProcessor.h"

App::App(const ushort port, const std::size_t blockSize) {
    _commandBundler = std::make_shared<CommandBundler>(blockSize); // send block size to constructor

    auto consoleMultiThreadProcessor = std::make_shared<MultiThreadProcessor>();
    consoleMultiThreadProcessor->addThread(std::make_unique<ConsoleThreadProcessor>("log"));
    _commandBundler->addProcessor(consoleMultiThreadProcessor);

    auto fileMultiThreadProcessor = std::make_shared<MultiThreadProcessor>();
    fileMultiThreadProcessor->addThread(std::make_unique<FileThreadProcessor>("file1"));
    fileMultiThreadProcessor->addThread(std::make_unique<FileThreadProcessor>("file2"));
    _commandBundler->addProcessor(fileMultiThreadProcessor);

    _requestHandlersFactory = std::make_shared<CommandsRequestHandlerFactory>(_commandBundler);

    _server = std::make_unique<Server>(_requestHandlersFactory, port);
}

void App::run() {
    _server->run();
}