#include "TablesManagerRequestHandlerFactory.h"

#include "TablesManagerRequestHandler.h"

TablesManagerRequestHandlerFactory::TablesManagerRequestHandlerFactory(
        const std::shared_ptr<ITablesManager>& tablesManager)
        : _tablesManager{tablesManager} {}

std::unique_ptr<IRequestHandler> TablesManagerRequestHandlerFactory::buildRequestHandler() {
    return std::make_unique<TablesManagerRequestHandler>(_tablesManager);
}