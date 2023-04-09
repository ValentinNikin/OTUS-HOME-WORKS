#include "async.h"

#include "Server.h"

using handle_t = async::handle_t;

auto serverPtr = Server::getInstance();

handle_t async::connect(std::size_t blockSize) {
    return serverPtr->connect(blockSize);
}

void async::receive(handle_t handle, const char *data, std::size_t size) {
    serverPtr->receive(handle, data, size);
}

void async::disconnect(handle_t handle) {
    serverPtr->disconnect(handle);
}
