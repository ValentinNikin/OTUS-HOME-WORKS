#pragma once

#include <memory>

#include <sqlite3.h>

#include "ITablesManager.h"
#include "db/DatabaseContext.h"

class DBTablesManager : public ITablesManager {
public:
    DBTablesManager(const std::shared_ptr<DatabaseContext>& dbContext);
    ~DBTablesManager() = default;

    void insert(const std::string& tableName, const Entity& item) override;
    void truncate(const std::string& tableName) override;
    std::vector<OperationResult> intersect() override;
    std::vector<OperationResult> symmetricDifference() override;

private:
    std::shared_ptr<DatabaseContext> _dbContext;
};
