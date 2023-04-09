#include "gtest/gtest.h"

#include <fstream>

#include "Config.h"

#include "classifiers/TensorflowClassifier.h"
#include "classifiers/ClassifierUtils.h"

constexpr std::size_t WIDTH = 28;
constexpr std::size_t HEIGHT = 28;

TEST(TensorflowClassifier, Simple) {
    auto testSamplesFilePath = Config::getInstance()->getResourcesPath() + "/test.csv";
    auto modelFolderPath = Config::getInstance()->getResourcesPath() + "/saved_model";

    TensorflowClassifier tc(modelFolderPath, WIDTH, HEIGHT);

    std::size_t countTests {0};
    std::size_t countPassedTests {0};

    std::string line;
    std::string substr;
    std::ifstream stream(testSamplesFilePath, std::ios_base::in);
    ASSERT_TRUE(stream.is_open());

    while (!stream.eof()) {
        std::getline(stream, line);
        if (line.empty()) continue;

        auto features = ClassifierUtils::extractFeatures(line, WIDTH, HEIGHT);
        auto predictedClass = tc.predict(features.second);

        if (predictedClass == features.first) {
            countPassedTests++;
        }

        countTests++;
    }

    EXPECT_NEAR(static_cast<double>(countPassedTests) / static_cast<double>(countTests), 0.909, 1e-6);
}