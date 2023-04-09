#pragma once

#include <string>
#include <memory>

#include <boost/regex.hpp>

struct Command {
    virtual ~Command() = default;
};

struct InsertCommand : Command {
    std::string tableName;
    std::size_t idColumn;
    std::string nameColumn;
};

struct TruncateCommand : Command {
    std::string tableName;
};

struct IntersectionCommand : Command {};

struct SymmetricDifferenceCommand : Command {};

class RequestParser {
public:
    RequestParser();

    std::unique_ptr<Command> parseRequest(const std::string& request);
private:
    boost::regex _insertCommandRegex;
    boost::regex _truncateCommandRegex;
    boost::regex _intersectionCommandRegex;
    boost::regex _symmetricDifferenceCommandRegex;
};
