#pragma once

#include <ostream>
#include <tuple>

namespace utils {
    /**
     * @brief Вывести tuple с разделителем "."
     * @tparam T тип tuple
     * @tparam Is индексы элементов tuple
     * @param os ostream для вывода tuple
     * @param tuple tuple
     */
    template<typename T, std::size_t... Is>
    void printTuple(std::ostream& os, const T& tuple, std::index_sequence<Is...>) {
        std::size_t idx = 0;
        auto printElement = [&idx, &os](const auto& x) {
            if (idx++ > 0) os << ".";
            os << x;
        };

        (printElement(std::get<Is>(tuple)), ...);

        os << std::endl;
    }

    /**
     * @brief Получить значение определенного байта
     * @tparam T тип значения
     * @param val значение
     * @param byteNum номер байта
     * @return значение байта
     */
    template<typename T>
    int takeByte(T val, std::size_t byteNum) {
        return (val >> (8*byteNum)) & 0xff;
    }
}