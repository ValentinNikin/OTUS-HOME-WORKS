#pragma once

#include <string>
#include <memory>

#include <sqlite3.h>

#include "DBTable.h"

class DatabaseContext {
public:
    DatabaseContext(const std::string& dbPath);

    DBTable* getTableA();
    DBTable* getTableB();

private:
    std::unique_ptr<DBTable> _tableA;
    std::unique_ptr<DBTable> _tableB;

    sqlite3* _connection;
};