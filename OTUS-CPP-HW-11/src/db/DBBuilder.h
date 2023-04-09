#pragma once

#include <string>

#include <sqlite3.h>

class DBBuilder {
public:
    DBBuilder(sqlite3* connection);

    bool buildSchemeIfNeed(std::string& errMsg);
private:
    sqlite3* _connection;
};
