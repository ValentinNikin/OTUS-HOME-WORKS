#pragma once

#include <bitset>

/**
 * @brief Динамический bitset
 *
 * Данный класс представляет собой попытку реализовать bitset,
 * размер которого можно задавать в runtime.
 *
 * Данные хранятся в массиве из bitset-ов.
 * Также определены некоторые свойственные bitset-у операции.
 */
struct DynamicBitset {
    DynamicBitset() = delete;

    /**
     * @brief Конструктор
     *
     * @param size желаемый размер bitset-а
     */
    explicit DynamicBitset(std::size_t size);

    ~DynamicBitset();

    /**
     * @brief Поиск диапазона элементов с значением false
     *
     * @param size размер искомого диапазона
     * @return позиция начала диапазона, либо SIZE_MAX - если диапазон не найден
     */
    std::size_t findUnsetContinuousRange(std::size_t size);

    /**
     * @brief Проверить бит
     * @param pos позиция бита
     * @return значение бита
     */
    bool test(std::size_t pos);

    /**
     * @brief Установить бит в значение true
     * @param pos позиция бита
     */
    void set(std::size_t pos);

    /**
     * @brief Установить диапазон битов в значение true
     * @param pos позиция первого бита диапазона
     * @param size количество битов в диапазоне
     */
    void set(std::size_t pos, std::size_t size);

    /**
     * @brief Установить бит в значение false
     * @param pos позиция бита
     */
    void reset(std::size_t pos);

    /**
     * @brief Установить диапазон битов в значение false
     * @param pos позиция первого бита диапазона
     * @param size количество битов в диапазоне
     */
    void reset(std::size_t pos, std::size_t size);

private:

    static constexpr uint8_t BITS_IN_BYTE = 8;

    std::bitset<BITS_IN_BYTE>* _bitsets;
    std::size_t _size; ///< размер bitset-а
};