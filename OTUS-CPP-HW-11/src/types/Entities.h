#pragma once

#include <string>

struct Entity {
    std::size_t id;
    std::string name;

    Entity() = default;
    Entity(const std::size_t id_, const std::string& name_)
        : id(id_), name(name_) {}
};

struct OperationResult {
    std::size_t id;
    std::string name1;
    std::string name2;
};