#pragma once

#include <string>
#include <vector>

#include "types/Entities.h"

class ITablesManager {
public:
    virtual ~ITablesManager() = default;

    virtual void insert(const std::string& tableName, const Entity& item) = 0;
    virtual void truncate(const std::string& tableName) = 0;
    virtual std::vector<OperationResult> intersect() = 0;
    virtual std::vector<OperationResult> symmetricDifference() = 0;
};