#include "DatabaseContext.h"

#include <stdexcept>

#include "DBBuilder.h"

DatabaseContext::DatabaseContext(const std::string& dbPath) {
    auto res = sqlite3_open_v2(dbPath.c_str(), &_connection,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
    if (res != SQLITE_OK) {
        sqlite3_close(_connection);
        throw std::runtime_error(std::string(sqlite3_errmsg(_connection)));
    }

    DBBuilder dbBuilder(_connection);
    std::string errMessage;
    if (!dbBuilder.buildSchemeIfNeed(errMessage)) {
        throw std::runtime_error(errMessage);
    }

    _tableA = std::make_unique<DBTable>("A", _connection);
    _tableB = std::make_unique<DBTable>("B", _connection);
}

DBTable* DatabaseContext::getTableA() {
    return _tableA.get();
}

DBTable* DatabaseContext::getTableB() {
    return _tableB.get();
}