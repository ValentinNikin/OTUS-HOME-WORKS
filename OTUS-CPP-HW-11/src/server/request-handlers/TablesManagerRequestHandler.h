#pragma once

#include <memory>

#include "server/IRequestHandler.h"
#include "tables-manager/ITablesManager.h"

class TablesManagerRequestHandler : public IRequestHandler {
public:
    TablesManagerRequestHandler(const std::shared_ptr<ITablesManager>& tablesManager);

    std::vector<std::string> processData(const char* data, std::size_t size) override;
private:
    std::shared_ptr<ITablesManager> _tablesManager;
};
