#pragma once

#include <vector>
#include <memory>
#include <map>

#include "async.h"
#include "processors/IBlockProcessor.h"
#include "Command.h"
#include "types/Block.h"
#include "Context.h"

class CommandBundler {
public:
    CommandBundler() = default;

    using handle_t = async::handle_t;

    handle_t createExecutionContext(std::size_t targetBlockSize);
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

    /// Отправить готовый блок команд в обработчики
    void sendBlockToProcessors(const std::shared_ptr<Block>& block);
};