#include "App.h"

#include <iostream>

#include "db/DatabaseContext.h"
#include "tables-manager/DBTablesManager.h"


App::App(const ushort port) {
    auto dbContext = std::make_shared<DatabaseContext>("./SQLITE_TEMP.db");
    _tablesManager = std::make_shared<DBTablesManager>(dbContext);

    _requestHandlersFactory = std::make_shared<TablesManagerRequestHandlerFactory>(_tablesManager);
    _server = std::make_unique<Server>(_requestHandlersFactory, port);
}

void App::run() {
    _server->run();
}