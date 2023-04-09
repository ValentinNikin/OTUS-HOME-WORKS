#include "UserReducer.h"

#include "../nlohmann-json/json.hpp"

#include "UserMapper.h"

std::string UserReducerResult::serialize() {
    nlohmann::json jsonArr = nlohmann::json::array();
    for (const auto& i : _prefixes) {
        nlohmann::json json;
        json["prefix"] = i.prefix;
        json["count"] = i.count;
        json["isFullWord"] = i.isFullWord;
        jsonArr.push_back(json);
    }
    return jsonArr.dump();
}

void UserReducerResult::deserialize(const std::string& str) {
    nlohmann::json jsonArr = nlohmann::json::parse(str);
    for (const auto& i : jsonArr.items()) {
        nlohmann::json json = i.value();
        PrefixStruct ps;
        ps.prefix = json["prefix"].get<std::string>();
        ps.count = json["count"].get<std::size_t>();
        ps.isFullWord = json["isFullWord"].get<bool>();
        _prefixes.push_back(ps);
    }
}

void UserReducerResult::addPrefix(const std::string& prefix, const std::size_t count, const bool isFullWord) {
    auto existItem = std::find_if(_prefixes.begin(), _prefixes.end(), [&prefix, &isFullWord](const auto& item) {
        return item.prefix == prefix && item.isFullWord == isFullWord;
    });

    if (existItem == _prefixes.end()) {
        _prefixes.emplace_back(prefix, count, isFullWord);
    }
    else {
        existItem->count += count;
    }
}

UserReducerResult::Prefixes UserReducerResult::getPrefixes() const {
    return _prefixes;
}

void UserReducer::reduce(
        IReducerResult* prevResult, const IMapperResult* mapperResult) {
    auto rRes = dynamic_cast<UserReducerResult*>(prevResult);
    auto mRes = dynamic_cast<const UserMapperResult*>(mapperResult);

    rRes->addPrefix(mRes->getPrefix(), mRes->getCount(), mRes->isFullWord());
}

std::unique_ptr<IReducerResult> UserReducer::makeInitialResult() const {
    return std::make_unique<UserReducerResult>();
}