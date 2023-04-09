#pragma once

#include <vector>
#include <memory>
#include <map>

#include "processors/IBlockProcessor.h"
#include "Command.h"
#include "types/Block.h"
#include "Context.h"

using handle_t = void*;

class CommandBundler {
public:
    CommandBundler(const std::size_t targetBlockSize);
    ~CommandBundler();

    handle_t createExecutionContext();
    void removeExecutionContext(handle_t handle);

    void processCommand(handle_t handle, std::unique_ptr<ICommand> command);

    void addProcessor(std::shared_ptr<IBlockProcessor> processor);
private:
    void collectCommand(Context* context, ContainerCommand* cmd);
    void startDynamicBlock(Context* context);
    void endDynamicBlock(Context* context);
    void eof(Context* context);
private:
    std::vector<std::shared_ptr<IBlockProcessor>> _processors;

    std::size_t _targetBlockSize;
    std::unique_ptr<Context> _commonContext;

    /// Отправить готовый блок команд в обработчики
    void sendBlockToProcessors(const std::shared_ptr<Block>& block);
};