#pragma once

#include <memory>
#include <string>

class IMapperResult {
public:
    virtual ~IMapperResult() = default;

    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& str) = 0;
    virtual bool less(const IMapperResult* other) const = 0;
    virtual bool equal(const IMapperResult* other) const = 0;

    bool operator<(const IMapperResult* otherRes) {
        return less(otherRes);
    }

    bool operator==(const IMapperResult* otherRes) {
        return equal(otherRes);
    }
};

class IMapper {
public:
    virtual ~IMapper() = default;
    virtual std::unique_ptr<IMapperResult> map(const std::string& str) = 0;
    virtual std::unique_ptr<IMapperResult> deserialize(const std::string& str) = 0;
};