#pragma once

#include <memory>
#include <set>
#include <mutex>

#include "Connection.h"

class Connection;

class ConnectionManager {
public:
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager() = default;

    void start(std::shared_ptr<Connection> connection);
    void stop(std::shared_ptr<Connection> connection);
    void stopAll();
private:
    std::set<std::shared_ptr<Connection>> _connections;
    std::mutex _mutex;
};
