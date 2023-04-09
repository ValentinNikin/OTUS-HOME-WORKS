#pragma once

#include <memory>
#include <stdlib.h>

#include "server/Server.h"

#include "tables-manager/ITablesManager.h"
#include "server/request-handlers/TablesManagerRequestHandlerFactory.h"

class App {
public:
    App(const ushort port);
    void run();
private:
    std::shared_ptr<Server> _server;
    std::shared_ptr<TablesManagerRequestHandlerFactory> _requestHandlersFactory;
    std::shared_ptr<ITablesManager> _tablesManager;
};
