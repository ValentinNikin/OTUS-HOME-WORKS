#include "DBBuilder.h"

#include <stdexcept>

DBBuilder::DBBuilder(sqlite3* connection)
    : _connection(connection) {}

bool DBBuilder::buildSchemeIfNeed(std::string& errMsg) {

    const std::string SQL_CREATE_A_QUERY = "CREATE TABLE IF NOT EXISTS A (id INTEGER PRIMARY KEY, name TEXT);";
    const std::string SQL_CREATE_B_QUERY = "CREATE TABLE IF NOT EXISTS B (id INTEGER PRIMARY KEY, name TEXT);";

    char* err;

    auto res = sqlite3_exec(_connection, SQL_CREATE_A_QUERY.c_str(), nullptr, nullptr, &err);
    if (res != SQLITE_OK) {
        errMsg = std::string(err);
        sqlite3_free(err);
        return false;
    }

    res = sqlite3_exec(_connection, SQL_CREATE_B_QUERY.c_str(), nullptr, nullptr, &err);
    if (res != SQLITE_OK) {
        errMsg = std::string(err);
        sqlite3_free(err);
        return false;
    }

    return true;
}