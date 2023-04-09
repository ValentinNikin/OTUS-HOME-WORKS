#include "UserMapper.h"

#include "../nlohmann-json/json.hpp"

UserMapperResult::UserMapperResult(const std::string& prefix, const std::size_t count, const bool isFullWord)
        : _prefix(prefix), _count(count), _isFullWord(isFullWord) {}

std::string UserMapperResult::serialize() const {
    nlohmann::json json;
    json["prefix"] = _prefix;
    json["count"] = _count;
    json["isFullWord"] = _isFullWord;
    return json.dump();
}

void UserMapperResult::deserialize(const std::string& str) {
    auto json = nlohmann::json::parse(str);
    _prefix = json["prefix"].get<std::string>();
    _count = json["count"].get<std::size_t>();
    _isFullWord = json["isFullWord"].get<bool>();
}

bool UserMapperResult::less(const IMapperResult* other) const {
    const auto casted = dynamic_cast<const UserMapperResult*>(other);
    return _prefix < casted->_prefix;
}

bool UserMapperResult::equal(const IMapperResult* other) const {
    const auto casted = dynamic_cast<const UserMapperResult*>(other);
    return _prefix == casted->_prefix;
}

std::string UserMapperResult::getPrefix() const {
    return _prefix;
}

std::size_t UserMapperResult::getCount() const {
    return _count;
}

bool UserMapperResult::isFullWord() const {
    return _isFullWord;
}

UserMapper::UserMapper(const std::size_t targetLength)
    : _targetLength(targetLength) {}

std::unique_ptr<IMapperResult> UserMapper::map(const std::string& str) {
    if (str.size() < _targetLength) {
        return std::make_unique<UserMapperResult>(str, 1, true);
    }
    return std::make_unique<UserMapperResult>(str.substr(0, _targetLength), 1, false);
}

std::unique_ptr<IMapperResult> UserMapper::deserialize(const std::string& str) {
    auto mRes = std::make_unique<UserMapperResult>();
    mRes->deserialize(str);
    return mRes;
}