#pragma once

#include <vector>

#include <sqlite3.h>

#include "types/Entities.h"

class DBTable {
public:
    DBTable(const std::string& name, sqlite3* connection);
    ~DBTable();

    std::string name() const;
    std::vector<Entity> list() const;
    void insert(const Entity& item);
    void truncate();
    std::vector<OperationResult> intersect(const DBTable* otherTable) const;
    std::vector<OperationResult> symmetricDifference(const DBTable* otherTable) const;
private:
    std::string _name;

    sqlite3* _connection;

    sqlite3_stmt* _listStmt;
    sqlite3_stmt* _insertStmt;
    sqlite3_stmt* _truncateStmt;
};
