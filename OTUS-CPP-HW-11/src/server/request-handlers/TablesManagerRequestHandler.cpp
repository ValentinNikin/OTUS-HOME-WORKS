#include "TablesManagerRequestHandler.h"

#include "utils/Utils.h"
#include "RequestParser.h"

TablesManagerRequestHandler::TablesManagerRequestHandler(
        const std::shared_ptr<ITablesManager>& tablesManager)
        : _tablesManager{tablesManager} {}

namespace {

std::string operationResultsToResponse(const std::vector<OperationResult>& results) {
    std::string response;
    for (const auto& r : results) {
        response += std::to_string(r.id) + "," + r.name1 + "," + r.name2 + "\n";
    }
    response += "OK\n";
    return response;
}

}

std::vector<std::string> TablesManagerRequestHandler::processData(
                        const char* data, std::size_t size) {
    std::string bufferStr;
    bufferStr.assign(data, size);

    RequestParser requestParser;

    std::vector<std::string> responses;

    auto requestCommands = Utils::split(bufferStr, '\n');
    for (const auto& rc : requestCommands) {
        if (rc.empty()) continue;

        try {
            auto command = requestParser.parseRequest(rc);

            if (auto insertCommand = dynamic_cast<InsertCommand *>(command.get())) {
                _tablesManager->insert(
                        insertCommand->tableName, Entity(insertCommand->idColumn, insertCommand->nameColumn));
                responses.push_back("OK\n");
            } else if (auto truncateCommand = dynamic_cast<TruncateCommand *>(command.get())) {
                _tablesManager->truncate(truncateCommand->tableName);
                responses.push_back("OK\n");
            } else if (dynamic_cast<IntersectionCommand*>(command.get())) {
                auto results = _tablesManager->intersect();
                responses.push_back(operationResultsToResponse(results));
            } else if (dynamic_cast<SymmetricDifferenceCommand*>(command.get())) {
                auto results = _tablesManager->symmetricDifference();
                responses.push_back(operationResultsToResponse(results));
            }
        }
        catch (const std::exception& exc) {
            responses.push_back("ERR " + std::string(exc.what()) + "\n");
            continue;
        }
    }

    return responses;
}
