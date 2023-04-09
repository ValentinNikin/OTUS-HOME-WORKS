#include "DuplicateFinder.h"

#include <boost/container_hash/hash.hpp>

DuplicateFinder::DuplicateFinder(
        const std::shared_ptr<HashCalculator>& hashCalculator, std::size_t blockSize)
        : _hashCalculator(hashCalculator), _blockSize(blockSize) {
    _fileComparator = std::make_unique<FileComparator>();
}

std::vector<std::vector<boost::filesystem::path>> DuplicateFinder::findDuplicates(
        const std::vector<FileInfo>& files) {

    // first - ссылка на структуру-обертку над файлом,
    // second - флаг, файл обработан
    using FilesWrappersCollection = std::vector<std::pair<std::unique_ptr<FileBlockHashReader>, bool>>;
    FilesWrappersCollection filesHashReaders;

    for (const auto& f : files) {
        auto fileHashReader = std::make_unique<FileBlockHashReader>(
                f, _hashCalculator, _blockSize);
        filesHashReaders.push_back(std::make_pair(std::move(fileHashReader), false));
    }

    std::vector<std::vector<boost::filesystem::path>> duplicatesGroups;

    /**
     * 1. Выделяем файл currentFileIt.
     * 2. Ищем дубликаты для currentFileIt.
     * 3. Если дубликаты найдены, то:
     *      - собираем пути к этим файлам в группу
     *      - помечаем все дубликаты как обработанные
     */
    for (auto currentFileIt = filesHashReaders.begin(); currentFileIt != filesHashReaders.end(); currentFileIt++) {
        if (currentFileIt->second) continue;

        std::vector<FilesWrappersCollection::iterator> duplicates;
        for (auto it = std::next(currentFileIt); it != filesHashReaders.end(); it++) {
            if (!it->second && _fileComparator->compare(currentFileIt->first.get(), it->first.get())) {
                duplicates.push_back(it);
            }
        }

        if (!duplicates.empty()) {
            std::vector<boost::filesystem::path> group;

            group.push_back(currentFileIt->first->getFilePath());

            for (const auto& duplicate : duplicates) {
                group.push_back(duplicate->first->getFilePath());
                duplicate->second = true;
            }

            duplicatesGroups.push_back(group);
            duplicates.clear();
        }

        currentFileIt->second = true;
    }

    return duplicatesGroups;
}