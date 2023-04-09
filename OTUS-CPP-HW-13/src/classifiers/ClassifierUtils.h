#pragma once

#include <fstream>
#include <sstream>

#include "Classifier.h"

namespace ClassifierUtils {

std::pair<std::size_t, Classifier::features_t> extractFeatures(
            const std::string& line, const std::size_t width, const std::size_t height) {
    if (line.empty()) {
        throw std::runtime_error("Line should not be empty");
    }

    std::istringstream iss(line);
    std::string substr;
    std::getline(iss, substr, ',');

    std::size_t classNum = static_cast<std::size_t>(atoi(substr.c_str()));
    Classifier::features_t features;
    features.reserve(width * height);

    while (std::getline(iss, substr, ',')) {
        features.push_back(static_cast<float>(atof(substr.c_str())));
    }

    return { classNum, features };
}

}