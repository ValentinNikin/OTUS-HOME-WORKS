#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <list>
#include <tuple>

#include "type_traits_extending.h"
#include "utils.h"

/// Специализация функции для целочисленных типов
template<typename T>
std::enable_if_t<std::is_integral<type_traits_extending::remove_cvref_t<T>>::value, void>
print_ip(T val) {
    auto size = sizeof(T);
    for (std::size_t i = size - 1; i > 0; i--) {
        std::cout << utils::takeByte(val, i) << ".";
    }
    std::cout << utils::takeByte(val, 0) << std::endl;
}

/// Специализация функции для std::vector и std::list
template<typename T>
std::enable_if_t<type_traits_extending::is_vector_or_list<T>::value, void>
print_ip(T val) {
    auto currentIt = std::begin(val);
    auto lastItem = --std::end(val);

    for (; currentIt != lastItem; currentIt++) {
        std::cout << *currentIt << ".";
    }
    std::cout << *currentIt << std::endl;
}

/// Специализация функции для std::string
template<typename T>
std::enable_if_t<type_traits_extending::is_string<T>::value, void>
print_ip(T val) {
    std::cout << val << std::endl;
}

/// Специализация функции для std::tuple
template<typename T>
std::enable_if_t<type_traits_extending::is_tuple<T>::value, void>
print_ip(T val) {
    utils::printTuple(std::cout, val, std::make_index_sequence<std::tuple_size_v<T>>{});
}

int main (int, char **) {
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} ); // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0

    return 0;
}
