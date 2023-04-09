#include "ConnectionManager.h"

void ConnectionManager::start(std::shared_ptr<Connection> connection) {
    std::lock_guard<std::mutex> lg(_mutex);
    _connections.insert(connection);
    connection->start();
}

void ConnectionManager::stop(std::shared_ptr<Connection> connection) {
    std::lock_guard<std::mutex> lg(_mutex);
    _connections.erase(connection);
    connection->stop();
}

void ConnectionManager::stopAll() {
    std::lock_guard<std::mutex> lg(_mutex);
    for (auto& c : _connections) {
        c->stop();
    }
    _connections.clear();
}