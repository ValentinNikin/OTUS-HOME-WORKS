#pragma once

#include <vector>

#include "map-reducer/include/IReducer.h"

struct PrefixStruct {
    std::string prefix;
    std::size_t count;
    bool isFullWord;

    PrefixStruct() = default;
    PrefixStruct(const std::string& prefix_, const std::size_t count_, const bool isFullWord_)
        : prefix(prefix_), count(count_), isFullWord(isFullWord_) {}
};

class UserReducerResult : public IReducerResult {
    using Prefixes = std::vector<PrefixStruct>;
public:
    std::string serialize() override;
    void deserialize(const std::string& str) override;

    void addPrefix(const std::string& prefix, const std::size_t count, const bool isFullWord);
    Prefixes getPrefixes() const;
private:
    Prefixes _prefixes;
};

class UserReducer : public IReducer {
public:
    void reduce(IReducerResult* prevResult, const IMapperResult* mapperResult) override;
    std::unique_ptr<IReducerResult> makeInitialResult() const override;
};