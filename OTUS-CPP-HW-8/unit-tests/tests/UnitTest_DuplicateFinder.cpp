#include "gtest/gtest.h"

#include "duplicate-finder/DuplicateFinder.h"
#include "duplicate-finder/hash/DefaultHashCalculator.h"
#include "duplicate-finder/hash/Crc32Calculator.h"
#include "file-scanner/FileScanner.h"

#include "../Config.h"

namespace {

std::vector<FileInfo> prepareFilesList() {
    auto fileScannerDir = Config::getInstance()->getResourcesPath() + "/duplicate-finder";
    std::vector<std::string> scanDirs = { fileScannerDir };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIsRecursiveScan(true);
    auto files = fileScanner.scan();

    std::sort(files.begin(), files.end(), FileInfoComparator());

    return files;
}

void makeChecks(const std::vector<std::vector<boost::filesystem::path>>& duplicatesGroup) {
    ASSERT_EQ(duplicatesGroup.size(), 3);

    ASSERT_EQ(duplicatesGroup[0].size(), 3);
    ASSERT_EQ(duplicatesGroup[0][0].filename(), "1byte.txt");
    ASSERT_EQ(duplicatesGroup[0][1].filename(), "1byte_duplicate.txt");
    ASSERT_EQ(duplicatesGroup[0][2].filename(), "1byte_duplicate_2.txt");

    ASSERT_EQ(duplicatesGroup[1].size(), 2);
    ASSERT_EQ(duplicatesGroup[1][0].filename(), "cpp-file.cpp");
    ASSERT_EQ(duplicatesGroup[1][1].filename(), "h-file-duplicate.h");

    ASSERT_EQ(duplicatesGroup[2].size(), 2);
    ASSERT_EQ(duplicatesGroup[2][0].filename(), "hello_world_duplicate");
    ASSERT_EQ(duplicatesGroup[2][1].filename(), "hello_world.txt");
}

}

TEST(DuplicateFinder, defaultHash_1byte) {
    auto files = prepareFilesList();

    auto hashCalculator = std::make_shared<DefaultHashCalculator>();
    DuplicateFinder duplicateFinder(hashCalculator, 1);
    auto duplicatesGroup = duplicateFinder.findDuplicates(files);

    makeChecks(duplicatesGroup);
}

TEST(DuplicateFinder, defaultHash_5byte) {
    auto files = prepareFilesList();

    auto hashCalculator = std::make_shared<DefaultHashCalculator>();
    DuplicateFinder duplicateFinder(hashCalculator, 5);
    auto duplicatesGroup = duplicateFinder.findDuplicates(files);

    makeChecks(duplicatesGroup);
}

TEST(DuplicateFinder, crc32_1byte) {
    auto files = prepareFilesList();

    auto hashCalculator = std::make_shared<Crc32Calculator>();
    DuplicateFinder duplicateFinder(hashCalculator, 1);
    auto duplicatesGroup = duplicateFinder.findDuplicates(files);

    makeChecks(duplicatesGroup);
}

TEST(DuplicateFinder, crc32_5byte) {
    auto files = prepareFilesList();

    auto hashCalculator = std::make_shared<Crc32Calculator>();
    DuplicateFinder duplicateFinder(hashCalculator, 5);
    auto duplicatesGroup = duplicateFinder.findDuplicates(files);

    makeChecks(duplicatesGroup);
}