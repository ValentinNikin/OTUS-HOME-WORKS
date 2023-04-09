#include "DBTablesManager.h"

#include <stdexcept>

DBTablesManager::DBTablesManager(const std::shared_ptr<DatabaseContext>& dbContext)
    : _dbContext(dbContext) {}

void DBTablesManager::insert(const std::string& tableName, const Entity& item) {
    if (tableName == "A") {
        _dbContext->getTableA()->insert(item);
    }
    else if (tableName == "B") {
        _dbContext->getTableB()->insert(item);
    }
    else {
        throw std::runtime_error("Table with name \"" + tableName + "\" not found");
    }
}

void DBTablesManager::truncate(const std::string& tableName) {
    if (tableName == "A") {
        _dbContext->getTableA()->truncate();
    }
    else if (tableName == "B") {
        _dbContext->getTableB()->truncate();
    }
    else {
        throw std::runtime_error("Table with name \"" + tableName + "\" not found");
    }
}

std::vector<OperationResult> DBTablesManager::intersect() {
    return _dbContext->getTableA()->intersect(_dbContext->getTableB());
}

std::vector<OperationResult> DBTablesManager::symmetricDifference() {
    return _dbContext->getTableA()->symmetricDifference(_dbContext->getTableB());
}