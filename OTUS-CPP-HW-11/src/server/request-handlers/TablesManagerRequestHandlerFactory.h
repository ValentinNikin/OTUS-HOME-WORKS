#pragma once

#include "server/IRequestHandlerFactory.h"
#include "tables-manager/ITablesManager.h"

class TablesManagerRequestHandlerFactory : public IRequestHandlerFactory {
public:
    TablesManagerRequestHandlerFactory(const std::shared_ptr<ITablesManager>& tablesManager);

    std::unique_ptr<IRequestHandler> buildRequestHandler() override;
private:
    std::shared_ptr<ITablesManager> _tablesManager;
};
