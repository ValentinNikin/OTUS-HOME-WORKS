#pragma once

#include <memory>
#include <mutex>

#include "async.h"

#include "command-bundler/CommandBundler.h"

class Server {
private:
    Server();
    static std::unique_ptr<Server> _instance;
    static std::mutex _mutex;
public:
    static Server* getInstance();

    Server(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;
public:
    using handle_t = async::handle_t;

    handle_t connect(std::size_t blockSize);
    void receive(handle_t handle, const char *data, std::size_t size);
    void disconnect(handle_t handle);

private:
    std::unique_ptr<CommandBundler> _commandBundler;
};

