#include "gtest/gtest.h"

#include "file-scanner/FileScanner.h"

#include "../Config.h"

namespace {
    bool fileIsFound(
            const std::vector<FileInfo>& files, const std::string& expectedFilename) {
        return std::any_of(
                files.cbegin(), files.cend(),
                [&expectedFilename](const auto& fi) {
                    return fi.path.filename() == expectedFilename;
                });
    }
}

TEST(FileScanner, scanDirs) {
    auto fileScannerDir = Config::getInstance()->getResourcesPath() + "/file-scanner";
    std::vector<std::string> scanDirs = { fileScannerDir };

    FileScanner fileScanner(scanDirs);
    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 1);

    ASSERT_TRUE(fileIsFound(files, "file.txt"));
}

TEST(FileScanner, scanDirs_recursive) {
    auto fileScannerDir = Config::getInstance()->getResourcesPath() + "/file-scanner";
    std::vector<std::string> scanDirs = { fileScannerDir };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIsRecursiveScan(true);

    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 7);

    ASSERT_TRUE(fileIsFound(files, "file.txt"));
    ASSERT_TRUE(fileIsFound(files, "file1.cpp"));
    ASSERT_TRUE(fileIsFound(files, "file1.h"));
    ASSERT_TRUE(fileIsFound(files, "file2.html"));
    ASSERT_TRUE(fileIsFound(files, "file2.css"));
    ASSERT_TRUE(fileIsFound(files, "file2.cSS"));
    ASSERT_TRUE(fileIsFound(files, "file3.txt"));
}

TEST(FileScanner, scanDirs_ignoreDirs) {
    std::vector<std::string> scanDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner" };
    std::vector<std::string> ignoreDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner/scan_folder_2" };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIgnoreDirs(ignoreDirs);
    fileScanner.setIsRecursiveScan(true);

    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 3);

    ASSERT_TRUE(fileIsFound(files, "file.txt"));
    ASSERT_TRUE(fileIsFound(files, "file1.cpp"));
    ASSERT_TRUE(fileIsFound(files, "file1.h"));
}

TEST(FileScanner, scanDirs_minFileSize) {
    std::vector<std::string> scanDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner" };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIsRecursiveScan(true);
    fileScanner.setMinFileSize(20);

    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 5);

    ASSERT_TRUE(fileIsFound(files, "file1.cpp"));
    ASSERT_TRUE(fileIsFound(files, "file3.txt"));
    ASSERT_TRUE(fileIsFound(files, "file2.css"));
    ASSERT_TRUE(fileIsFound(files, "file2.cSS"));
    ASSERT_TRUE(fileIsFound(files, "file2.html"));
}

TEST(FileScanner, scanDirs_filesMasks) {
    std::vector<std::string> scanDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner" };
    std::vector<std::string> filesMasks = { "file?.c??", "*1.*" };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIsRecursiveScan(true);
    fileScanner.setFilesMasks(filesMasks);

    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 4);

    ASSERT_TRUE(fileIsFound(files, "file1.cpp"));
    ASSERT_TRUE(fileIsFound(files, "file1.h"));
    ASSERT_TRUE(fileIsFound(files, "file2.css"));
    ASSERT_TRUE(fileIsFound(files, "file2.cSS"));
}

TEST(FileScanner, scanDirs_combo) {
    std::vector<std::string> scanDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner" };
    std::vector<std::string> ignoreDirs =
            { Config::getInstance()->getResourcesPath() + "/file-scanner/scan_folder_2/scan_folder_3" };
    std::vector<std::string> filesMasks = { "file?.c??", "*1.*", "file3.*" };

    FileScanner fileScanner(scanDirs);
    fileScanner.setIsRecursiveScan(true);
    fileScanner.setFilesMasks(filesMasks);
    fileScanner.setIgnoreDirs(ignoreDirs);
    fileScanner.setMinFileSize(20);

    auto files = fileScanner.scan();

    ASSERT_EQ(files.size(), 3);

    ASSERT_TRUE(fileIsFound(files, "file1.cpp"));
    ASSERT_TRUE(fileIsFound(files, "file2.css"));
    ASSERT_TRUE(fileIsFound(files, "file2.cSS"));
}