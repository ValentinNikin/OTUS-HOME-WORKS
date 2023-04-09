#pragma once

#include "map-reducer/include/IMapper.h"

class UserMapperResult : public IMapperResult {
public:
    UserMapperResult() = default;
    UserMapperResult(const std::string& prefix, const std::size_t count, const bool isFullWord);

    std::string serialize() const override;
    void deserialize(const std::string& str) override;
    bool less(const IMapperResult* other) const override;
    bool equal(const IMapperResult* other) const override;

    std::string getPrefix() const;
    std::size_t getCount() const;
    bool isFullWord() const;
private:
    std::string _prefix;
    std::size_t _count;
    bool _isFullWord {false};
};


class UserMapper : public IMapper {
public:
    UserMapper(const std::size_t targetLength);

    std::unique_ptr<IMapperResult> map(const std::string& str) override;
    std::unique_ptr<IMapperResult> deserialize(const std::string& str) override;
private:
    std::size_t _targetLength;
};