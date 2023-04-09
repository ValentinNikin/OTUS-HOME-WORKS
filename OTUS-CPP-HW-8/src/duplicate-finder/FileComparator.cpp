#include "FileComparator.h"

bool FileComparator::compare(FileBlockHashReader* file1, FileBlockHashReader* file2) {
    // если размер не совпадает, дальнейшие сравнения не имеют смысла
    if (file1->getSize() != file2->getSize()) return false;

    if (!file1->isOpen()) {
        file1->open();
    }

    if (!file2->isOpen()) {
        file2->open();
    }

    // для обоих файлов сбрасываем счетчик индекса блока в начало
    file1->reset();
    file2->reset();

    std::size_t blockHash1;
    std::size_t blockHash2;
    bool blockIsRead1 = true;
    bool blockIsRead2 = true;

    do {
        blockIsRead1 = file1->readNextBlock(blockHash1);
        blockIsRead2 = file2->readNextBlock(blockHash2);
    } while (blockIsRead1 && blockIsRead2 && blockHash1 == blockHash2);

    /**
     * Если не удалось считать блок ни из одного из файлов, это означает, что оба файла
     * завершились. Если хэш последних блоков совпадает, считаем, что файлы идентичные.
     */
    return !blockIsRead1 && !blockIsRead2 && blockHash1 == blockHash2;
}