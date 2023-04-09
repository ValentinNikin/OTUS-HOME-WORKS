#pragma once

#include <string>
#include <memory>

#include "IMapper.h"

class IReducerResult {
public:
    virtual ~IReducerResult() = default;
    virtual std::string serialize() = 0;
    virtual void deserialize(const std::string& str) = 0;
};

class IReducer {
public:
    virtual ~IReducer() = default;
    virtual void reduce(IReducerResult* prevResult, const IMapperResult* mapperResult) = 0;
    virtual std::unique_ptr<IReducerResult> makeInitialResult() const = 0;
};