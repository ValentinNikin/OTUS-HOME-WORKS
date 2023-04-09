#include "gtest/gtest.h"

#include <memory>

#include "processors/IBlockProcessor.h"
#include "command-bundler/Command.h"
#include "command-bundler/CommandBundler.h"

class MockProcessor : public IBlockProcessor {
public:
    MockProcessor(std::size_t expectedCommandsCount)
        : _expectedCommandsCount(expectedCommandsCount) {}

    void processBlock(std::shared_ptr<Block> block) override {
        ASSERT_EQ(block->getCommands().size(), _expectedCommandsCount);
    }

private:
    std::size_t _expectedCommandsCount;
};

TEST(CommandBundler, NormalBlock_1) {
    auto commandBundler = std::make_unique<CommandBundler>();
    auto context = commandBundler->createExecutionContext(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(3));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd3"));
    commandBundler->removeExecutionContext(context);
}

TEST(CommandBundler, NormalBlock_2) {
    auto commandBundler = std::make_unique<CommandBundler>();
    auto context = commandBundler->createExecutionContext(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(2));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(context, std::make_unique<EOFCommand>());
    commandBundler->removeExecutionContext(context);
}

TEST(CommandBundler, DynamicBlock_1) {
    auto commandBundler = std::make_unique<CommandBundler>();
    auto context = commandBundler->createExecutionContext(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(6));
    commandBundler->processCommand(context, std::make_unique<StartBlockCommand>());
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(context, std::make_unique<StartBlockCommand>());
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd3"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd4"));
    commandBundler->processCommand(context, std::make_unique<EndBlockCommand>());
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd5"));
    commandBundler->processCommand(context, std::make_unique<ContainerCommand>("cmd6"));
    commandBundler->processCommand(context, std::make_unique<EndBlockCommand>());
    commandBundler->removeExecutionContext(context);
}