#include "gtest/gtest.h"

#include <memory>

#include "processors/ICommandProcessor.h"
#include "commands/Command.h"
#include "CommandBundler.h"

class MockProcessor : public ICommandProcessor {
public:
    MockProcessor(std::size_t expectedCommandsCount)
        : _expectedCommandsCount(expectedCommandsCount) {}

    void processCommands(std::shared_ptr<Block> block) override {
        ASSERT_EQ(block->getCommands().size(), _expectedCommandsCount);
    }

private:
    std::size_t _expectedCommandsCount;
};

TEST(CommandBundler, Constructor) {
    ASSERT_THROW(std::make_unique<CommandBundler>(0), std::runtime_error);
}

TEST(CommandBundler, isEof) {
    auto commandBundler = std::make_unique<CommandBundler>(3);
    commandBundler->processCommand(std::make_unique<EOFCommand>());

    ASSERT_TRUE(commandBundler->isEof());
    ASSERT_THROW(commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd1")), std::runtime_error);
}

TEST(CommandBundler, NormalBlock_1) {
    auto commandBundler = std::make_unique<CommandBundler>(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(3));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd3"));
}

TEST(CommandBundler, NormalBlock_2) {
    auto commandBundler = std::make_unique<CommandBundler>(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(2));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(std::make_unique<EOFCommand>());
}

TEST(CommandBundler, DynamicBlock_1) {
    auto commandBundler = std::make_unique<CommandBundler>(3);
    commandBundler->addProcessor(std::make_shared<MockProcessor>(6));
    commandBundler->processCommand(std::make_unique<StartBlockCommand>());
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd1"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd2"));
    commandBundler->processCommand(std::make_unique<StartBlockCommand>());
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd3"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd4"));
    commandBundler->processCommand(std::make_unique<EndBlockCommand>());
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd5"));
    commandBundler->processCommand(std::make_unique<ContainerCommand>("cmd6"));
    commandBundler->processCommand(std::make_unique<EndBlockCommand>());
}