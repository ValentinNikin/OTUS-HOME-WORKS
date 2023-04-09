#include <iostream>
#include <fstream>

#include "classifiers/TensorflowClassifier.h"
#include "classifiers/ClassifierUtils.h"

constexpr std::size_t WIDTH = 28;
constexpr std::size_t HEIGHT = 28;

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "You should run application with 3 arguments, for example: ./fashio_mnist test.csv model" << std::endl;
        return 1;
    }

    std::string testSamplesFilePath = argv[1];
    std::string modelFolderPath = argv[2];

    TensorflowClassifier tc(modelFolderPath, WIDTH, HEIGHT);

    std::size_t countTests {0};
    std::size_t countPassedTests {0};

    std::string line;
    std::string substr;
    std::ifstream stream(testSamplesFilePath, std::ios_base::in);
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

    std::cout << static_cast<double>(countPassedTests) / static_cast<double>(countTests) << std::endl;

    return 0;
}