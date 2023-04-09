#include "RequestParser.h"

#include <boost/regex.hpp>

RequestParser::RequestParser()
    : _insertCommandRegex("^INSERT (\\w+) (\\d+) (\\w+)$"),
      _truncateCommandRegex("^TRUNCATE (\\w+)$"),
      _intersectionCommandRegex("^INTERSECTION$"),
      _symmetricDifferenceCommandRegex("^SYMMETRIC_DIFFERENCE$") {}

std::unique_ptr<Command> RequestParser::parseRequest(const std::string& request) {
    boost::smatch xResults;
    if (boost::regex_match(request, xResults, _insertCommandRegex)) {
        auto insertCommand = std::make_unique<InsertCommand>();
        insertCommand->tableName = xResults[1].str();
        insertCommand->idColumn = std::stoi(xResults[2].str());
        insertCommand->nameColumn = xResults[3].str();
        return insertCommand;
    }
    else if (boost::regex_match(request, xResults, _truncateCommandRegex)) {
        auto truncateCommand = std::make_unique<TruncateCommand>();
        truncateCommand->tableName = xResults[1].str();
        return truncateCommand;
    }
    else if (boost::regex_match(request, xResults, _intersectionCommandRegex)) {
        return std::make_unique<IntersectionCommand>();
    }
    else if (boost::regex_match(request, xResults, _symmetricDifferenceCommandRegex)) {
        return std::make_unique<SymmetricDifferenceCommand>();
    }
    throw std::runtime_error("Unsupported command");
}