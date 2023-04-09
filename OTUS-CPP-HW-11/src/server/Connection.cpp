#include "Connection.h"

#include <iostream>

Connection::Connection(
        tcp::socket socket,
        const std::shared_ptr<ConnectionManager>& connectionManager,
        std::unique_ptr<IRequestHandler> requestHandler)
        : _socket{std::move(socket)},
        _connectionManager{connectionManager},
        _requestHandler{std::move(requestHandler)} {}

void Connection::start() {
    readFromSocket();
}

void Connection::stop() {
    if (_socket.is_open()) {
        _socket.close();
    }
}

void Connection::readFromSocket() {
    auto self(shared_from_this());
    _socket.async_read_some(
            boost::asio::buffer(_buffer, _bufferSize),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    auto response = _requestHandler->processData(_buffer.data(), length);
                    writeToSocket(response);
                }
                else {
                    if (auto cm = _connectionManager.lock()) {
                        cm->stop(shared_from_this());
                    }
                    else {
                        stop();
                    }
                }
            });
}

void Connection::writeToSocket(const std::vector<std::string>& response) {
    auto self(shared_from_this());

    std::vector<boost::asio::const_buffer> buffers;
    buffers.reserve(response.size());

    std::transform(response.begin(), response.end(), std::back_inserter(buffers),
        [](const auto& r) {
            return boost::asio::buffer(r);
        });

    _socket.async_write_some(buffers, [this, self](boost::system::error_code ec, std::size_t) {
        if (ec) {
            std::cout << "Problem occurred while writing to socket: " << ec.message() << std::endl;
        }

        if (auto cm = _connectionManager.lock()) {
            cm->stop(shared_from_this());
        } else {
            stop();
        }
    });
}
