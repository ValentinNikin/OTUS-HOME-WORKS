#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "ConnectionManager.h"
#include "IRequestHandler.h"

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {
    using tcp = boost::asio::ip::tcp;
public:
    Connection(
            tcp::socket socket,
            const std::shared_ptr<ConnectionManager>& connectionManager,
            std::unique_ptr<IRequestHandler> requestHandler);

    void start();
    void stop();

private:
    void readFromSocket();

    tcp::socket _socket;

    std::weak_ptr<ConnectionManager> _connectionManager;
    std::unique_ptr<IRequestHandler> _requestHandler;

    static constexpr std::size_t _bufferSize {1000};
    std::array<char, _bufferSize> _buffer;
};