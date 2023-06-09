#include "CommandBundler.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

#include "Context.h"

CommandBundler::CommandBundler(std::size_t targetBlockSize)
    : _targetBlockSize{targetBlockSize},
    _commonContext{std::make_unique<Context>(targetBlockSize)} {}

CommandBundler::~CommandBundler() {
    eof(_commonContext.get());
}

handle_t CommandBundler::createExecutionContext() {
    return static_cast<handle_t>(new Context(_targetBlockSize));
}

void CommandBundler::removeExecutionContext(handle_t handle) {
    auto context = static_cast<Context*>(handle);
    eof(context);
    delete context;
}

void CommandBundler::addProcessor(std::shared_ptr<IBlockProcessor> processor) {
    _processors.push_back(processor);
}

void CommandBundler::processCommand(handle_t handle, std::unique_ptr<ICommand> command) {
    auto context = static_cast<Context*>(handle);

    if (auto cmd = dynamic_cast<ContainerCommand*>(command.get())) {
        collectCommand(context, cmd);
    }
    else if (dynamic_cast<StartBlockCommand*>(command.get())) {
        startDynamicBlock(context);
    }
    else if (dynamic_cast<EndBlockCommand*>(command.get())) {
        endDynamicBlock(context);
    }
    else if (dynamic_cast<EOFCommand*>(command.get())) {
        eof(context);
    }
    else {
        throw std::runtime_error("Unsupported command");
    }
}

void CommandBundler::collectCommand(Context* context, ContainerCommand* cmd) {
    std::unique_lock<std::mutex> contextUl(context->mutex);

    if (context->getState() == State::DynamicBlock) {
        context->addCommandToBlock(cmd->getPayload());

        if (context->blockIsReady()) {
            auto block = context->resetBlock();
            contextUl.unlock();

            sendBlockToProcessors(std::shared_ptr<Block>(block.release()));
        }
    }
    else if (context->getState() == State::NormalBlock) {
        contextUl.unlock();

        std::unique_lock<std::mutex> commonContextUl(_commonContext->mutex);
        _commonContext->addCommandToBlock(cmd->getPayload());
        if (_commonContext->blockIsReady()) {
            auto block = _commonContext->resetBlock();
            commonContextUl.unlock();

            sendBlockToProcessors(std::shared_ptr<Block>(block.release()));
        }
    }
}

void CommandBundler::startDynamicBlock(Context* context) {
    std::unique_lock<std::mutex> ul(context->mutex);

    if (context->getState() == State::NormalBlock) {
        // Принудительно завершаем предыдущий блок команд
        auto block = context->resetBlock();
        context->incDynamicBlockNestingLevel();
        ul.unlock();

        sendBlockToProcessors(std::shared_ptr<Block>(block.release()));
    }
    else {
        context->incDynamicBlockNestingLevel();
    }
}

void CommandBundler::endDynamicBlock(Context* context) {
    std::unique_lock<std::mutex> ul(context->mutex);

    if (context->getState() == State::NormalBlock) {
        assert(0 && "Dynamic block wasn't open");
        std::cerr << "Dynamic block wasn't open" << std::endl;
        return;
    }

    context->decDynamicBlockNestingLevel();

    if (context->getState() == State::NormalBlock) {
        auto block = context->resetBlock();
        ul.unlock();

        sendBlockToProcessors(std::shared_ptr<Block>(block.release()));
    }
}

void CommandBundler::eof(Context* context) {
    std::unique_lock<std::mutex> ul(context->mutex);

    auto lastState = context->getState();
    context->setState(State::Eof);

    if (lastState == State::NormalBlock) {
        auto block = context->resetBlock();
        ul.unlock();

        sendBlockToProcessors(std::shared_ptr<Block>(block.release()));
    }
}

void CommandBundler::sendBlockToProcessors(const std::shared_ptr<Block>& block) {
    if (block->empty()) {
        return;
    }

    for (const auto& p : _processors) {
        p->processBlock(block);
    }
}

