#include "DBTable.h"

#include <stdexcept>

#include <boost/format.hpp>

DBTable::DBTable(
        const std::string& name, sqlite3* connection)
        : _name(name), _connection(connection) {

    const std::string SQL_LIST_QUERY = "SELECT * FROM " + _name + ";";
    const std::string SQL_INSERT_QUERY = "INSERT INTO " + _name + " (id, name) VALUES (?1, ?2);";
    const std::string SQL_TRUNCATE_QUERY = "DELETE FROM " + _name + ";";

    int res;
    if ((res = sqlite3_prepare_v3(_connection,
                       SQL_LIST_QUERY.c_str(),
                       SQL_LIST_QUERY.size(),
                       SQLITE_PREPARE_PERSISTENT,
                       &_listStmt,
                       nullptr)) != SQLITE_OK) {
        throw std::runtime_error("Error occurred while preparing list request: " + std::string(sqlite3_errmsg(_connection)));
    };

    if ((res = sqlite3_prepare_v3(_connection,
                       SQL_INSERT_QUERY.c_str(),
                       SQL_INSERT_QUERY.size(),
                       SQLITE_PREPARE_PERSISTENT,
                       &_insertStmt,
                       nullptr)) != SQLITE_OK) {
        throw std::runtime_error("Error occurred while preparing insert request: " + std::string(sqlite3_errmsg(_connection)));
    }

    if ((res = sqlite3_prepare_v3(_connection,
                       SQL_TRUNCATE_QUERY.c_str(),
                       SQL_TRUNCATE_QUERY.size(),
                       SQLITE_PREPARE_PERSISTENT,
                       &_truncateStmt,
                       nullptr)) != SQLITE_OK) {
        throw std::runtime_error("Error occurred while preparing truncate request: " + std::string(sqlite3_errmsg(_connection)));
    }
}

DBTable::~DBTable() {
    sqlite3_finalize(_listStmt);
    sqlite3_finalize(_insertStmt);
    sqlite3_finalize(_truncateStmt);
}

std::string DBTable::name() const {
    return _name;
}

std::vector<Entity> DBTable::list() const {
    std::vector<Entity> entities;

    sqlite3_reset(_listStmt);

    int res;

    while ((res = sqlite3_step(_listStmt)) == SQLITE_ROW) {
        std::size_t id = sqlite3_column_int(_listStmt, 0);

        std::string name;
        if (sqlite3_column_type(_listStmt, 1) != SQLITE_NULL) {
            name = reinterpret_cast<const char *>(sqlite3_column_text(_listStmt, 1));
        }

        entities.push_back(Entity{id, name});
    }

    if (res != SQLITE_DONE) {
        throw std::runtime_error("Error occurred while executing list request: " + std::string(sqlite3_errmsg(_connection)));
    }

    return entities;
}

void DBTable::insert(const Entity& item) {
    sqlite3_reset(_insertStmt);
    sqlite3_clear_bindings(_insertStmt);

    if (sqlite3_bind_int(_insertStmt, 1, item.id) != SQLITE_OK) {
        throw std::runtime_error("Unable to bind id parameter to SQL statement: " + std::string(sqlite3_errmsg(_connection)));
    }

    if (sqlite3_bind_text(_insertStmt, 2, item.name.c_str(), item.name.size(), nullptr) != SQLITE_OK) {
        throw std::runtime_error("Unable to bind name parameter to SQL statement: " +
                                        std::string(sqlite3_errmsg(_connection)));
    }

    if (sqlite3_step(_insertStmt) != SQLITE_DONE) {
        throw std::runtime_error("Unable to execute insert query: " + std::string(sqlite3_errmsg(_connection)));
    }
}

void DBTable::truncate() {
    sqlite3_reset(_truncateStmt);

    if (sqlite3_step(_truncateStmt) != SQLITE_DONE) {
        throw std::runtime_error("Unable to execute truncate query: " + std::string(sqlite3_errmsg(_connection)));
    }
}

std::vector<OperationResult> DBTable::intersect(const DBTable* otherTable) const {
    std::vector<OperationResult> results;

    sqlite3_stmt* intersectStmt;
    const std::string SQL_INTERSECT_QUERY = boost::str(boost::format(
            "SELECT %1%.id, %1%.name as %1%, %2%.name as %2% "
            "FROM %1% "
            "INNER JOIN %2% "
            "WHERE %1%.id = %2%.id "
            "ORDER BY %1%.id;") % name() % otherTable->name());
    int res = sqlite3_prepare_v3(_connection, SQL_INTERSECT_QUERY.c_str(), SQL_INTERSECT_QUERY.size(),
                       SQLITE_PREPARE_PERSISTENT, &intersectStmt, nullptr);

    if (res != SQLITE_OK) {
        throw std::runtime_error("Error occurred while preparing intersect request: " + std::string(sqlite3_errmsg(_connection)));
    }

    while ((res = sqlite3_step(intersectStmt)) == SQLITE_ROW) {
        std::size_t id = sqlite3_column_int(intersectStmt, 0);
        std::string column1;
        std::string column2;

        if (sqlite3_column_type(intersectStmt, 1) != SQLITE_NULL) {
            column1 = reinterpret_cast<const char *>(sqlite3_column_text(intersectStmt, 1));
        }
        if (sqlite3_column_type(intersectStmt, 2) != SQLITE_NULL) {
            column2 = reinterpret_cast<const char *>(sqlite3_column_text(intersectStmt, 2));
        }

        results.push_back(OperationResult{id, column1, column2});
    }

    if (res != SQLITE_DONE) {
        throw std::runtime_error("Error occurred while executing intersect request: " + std::string(sqlite3_errmsg(_connection)));
    }

    sqlite3_finalize(intersectStmt);

    return results;
}

std::vector<OperationResult> DBTable::symmetricDifference(const DBTable* otherTable) const {
    std::vector<OperationResult> results;

    sqlite3_stmt* symmetricDiffStmt;
    // FULL OUTER JOIN is not supported in my sqlite 3.34
    const std::string SQL_SYMMETRIC_DIFFERENCE_QUERY = boost::str(boost::format(
        "SELECT id, A, B FROM ("
            "SELECT COALESCE(%1%.id, %2%.id) as id, %1%.name as A, %2%.name as B "
            "FROM %1% LEFT JOIN %2% USING(id) "
            "UNION ALL "
            "SELECT COALESCE(%1%.id, %2%.id) as id, %1%.name as A, %2%.name as B "
            "FROM %2% LEFT JOIN %1% USING(id)) req "
           "WHERE A IS NULL OR B IS NULL ORDER BY id;") % name() % otherTable->name());
    int res = sqlite3_prepare_v3(_connection, SQL_SYMMETRIC_DIFFERENCE_QUERY.c_str(), SQL_SYMMETRIC_DIFFERENCE_QUERY.size(),
                       SQLITE_PREPARE_PERSISTENT, &symmetricDiffStmt, nullptr);

    if (res != SQLITE_OK) {
        throw std::runtime_error("Error occurred while preparing symmetric difference request: " + std::string(sqlite3_errmsg(_connection)));
    }

    while ((res = sqlite3_step(symmetricDiffStmt)) == SQLITE_ROW) {
        std::size_t id = sqlite3_column_int(symmetricDiffStmt, 0);
        std::string column1;
        std::string column2;

        if (sqlite3_column_type(symmetricDiffStmt, 1) != SQLITE_NULL) {
            column1 = std::string(reinterpret_cast<const char *>(sqlite3_column_text(symmetricDiffStmt, 1)));
        }
        if (sqlite3_column_type(symmetricDiffStmt, 2) != SQLITE_NULL) {
            column2 = std::string(reinterpret_cast<const char*>(sqlite3_column_text(symmetricDiffStmt, 2)));
        }

        results.push_back(OperationResult{id, column1, column2});
    }

    if (res != SQLITE_DONE) {
        throw std::runtime_error("Error occurred while executing symmetric difference request: " + std::string(sqlite3_errmsg(_connection)));
    }

    sqlite3_finalize(symmetricDiffStmt);

    return results;
}