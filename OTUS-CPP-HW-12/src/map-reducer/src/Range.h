#pragma once

#include <cstdlib>

struct Range {
    std::size_t left;
    std::size_t right;

    Range(const std::size_t left_, const std::size_t right_)
            : left(left_), right(right_) {}
};