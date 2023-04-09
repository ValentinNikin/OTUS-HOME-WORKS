#include "Server.h"

#include <iostream>

#include "command-bundler/Command.h"
#include "Connection.h"

using tcp = boost::asio::ip::tcp;

Server::Server(
        const std::shared_ptr<IRequestHandlerFactory>& requestHandlerFactory,
        const ushort port,
        const std::size_t workersThreadsCount)
    : _workersThreadsCount{workersThreadsCount},
      _tcpAcceptor{_ioContext},
      _signalSet{_ioContext},
      _connectionManager{std::make_shared<ConnectionManager>()},
      _requestHandlerFactory{requestHandlerFactory} {
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    awaitStop();

    tcp::endpoint ep(tcp::v4(), port);
    _tcpAcceptor.open(ep.protocol());
    _tcpAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _tcpAcceptor.bind(ep);
    _tcpAcceptor.listen();

    acceptNewConnection();
}

Server::~Server() {
    for (std::size_t i = 0; i < _workersThreadsCount; i++) {
        if (_workersThreads[i].joinable()) {
            _workersThreads[i].join();
        }
    }
}

void Server::run() {
    _workersThreads.reserve(_workersThreadsCount);
    for (std::size_t i = 0; i < _workersThreadsCount; i++) {
        std::thread t(&Server::runEventProcessingLoop, this);
        _workersThreads.push_back(std::move(t));
    }

    runEventProcessingLoop();
}

void Server::acceptNewConnection() {
    _tcpAcceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!_tcpAcceptor.is_open()) {
                return;
            }

            if (!ec) {
                _connectionManager->start(
                        std::make_shared<Connection>(
                                std::move(socket),
                                _connectionManager,
                                _requestHandlerFactory->buildRequestHandler()));
            }

            acceptNewConnection();
        });
}

void Server::awaitStop() {
    _signalSet.async_wait([&](auto, auto){
        std::cout << "Catch exit event. Stop listening" << std::endl;
        _tcpAcceptor.close();
        _connectionManager->stopAll();
    });
}

void Server::runEventProcessingLoop() {
    _ioContext.run();
}