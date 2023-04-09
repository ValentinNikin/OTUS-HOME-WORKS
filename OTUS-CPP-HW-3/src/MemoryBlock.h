#pragma once

#include <cstdlib>

#include <DynamicBitset.h>

/**
 * @brief Блок памяти
 *
 * Резервирует память под заданное количество элементов.
 * Позволяет взаимодействовать с отдельными частями зарезервированной памяти.
 *
 * Память выделяется только 1 раз в конструкторе, и очищается 1 раз в деструкторе.
 * Менеджмент осуществляется за счет использования вспомогательного bitset-а,
 * в котором каждая из "ячеек" помечается 1 или 0 в зависимости от того занята
 * она или нет.
 *
 * @tparam ITEM_TYPE тип объектов, которые хранятся в зарезервированной памяти
 */
template<typename ITEM_TYPE>
struct MemoryBlock {
    MemoryBlock() = delete;

    /**
     * @brief Конструктор
     *
     * @param size количество объектов, которые может хранить данный блок
     */
    MemoryBlock(std::size_t size);

    ~MemoryBlock();

    /**
     * @brief Выделить кусок памяти для хранения заданного количества объектов
     * @param needSize количество объектов
     * @return указатель на выделенный кусок памяти, nullptr - в случае, если
     * не удалось выделить
     */
    ITEM_TYPE* allocateRange(std::size_t needSize);

    /**
     * @brief Вернуть кусок памяти в резерв
     * @param ptr указатель на начало куска памяти
     * @param size количество объектов
     * @return true - удалось вернуть, false - в противном случае (например, если
     * ptr не принадлежал текущему зарезервированному блоку)
     */
    bool freeRange(ITEM_TYPE* ptr, std::size_t size);

    /**
     * @return размер зарезервированного блока
     */
    std::size_t getSize() const { return _size; }

private:
    ITEM_TYPE* _data;
    std::size_t _size;
    DynamicBitset* _busyItems;
};

template<typename ITEM_TYPE>
MemoryBlock<ITEM_TYPE>::MemoryBlock(std::size_t size) {
    _data = new ITEM_TYPE[size];
    _size = size;
    _busyItems = new DynamicBitset(size);
}

template<typename ITEM_TYPE>
MemoryBlock<ITEM_TYPE>::~MemoryBlock() {
    delete[] _data;
    delete _busyItems;
}

template<typename ITEM_TYPE>
ITEM_TYPE* MemoryBlock<ITEM_TYPE>::allocateRange(std::size_t needSize) {
    if (needSize == 0) return nullptr;

    auto pos = _busyItems->findUnsetContinuousRange(needSize);
    if (pos == SIZE_MAX) {
        return nullptr;
    }

    _busyItems->set(pos, needSize);

    return _data + pos;
}

template<typename ITEM_TYPE>
bool MemoryBlock<ITEM_TYPE>::freeRange(ITEM_TYPE* ptr, std::size_t size) {
    auto currentPos = _data;
    for (std::size_t i = 0; i < _size; i++) {
        if (currentPos == ptr) {
            _busyItems->reset(i, size);
            return true;
        }
        currentPos++;
    }
    return false;
}