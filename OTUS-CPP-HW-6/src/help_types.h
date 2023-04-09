#pragma once

#include <tuple>
#include <stdint.h>

template<typename T, unsigned N, typename... REST>
struct generate_tuple_type
{
    typedef typename generate_tuple_type<T, N - 1, T, REST...>::type type;
};

template<typename T, typename... REST>
struct generate_tuple_type<T, 0, REST...>
{
    typedef std::tuple<REST...> type;
};

template<typename T, uint16_t N>
using tuple_N = typename generate_tuple_type<T, N>::type;

template <typename T1, typename T2, std::size_t ...I>
void copy_tuple_impl(T1 const & from, T2 & to, std::index_sequence<I...>)
{
    int dummy[] = { (std::get<I>(to) = std::get<I>(from), 0)... };
    static_cast<void>(dummy);
}

template <typename T1, typename T2>
void copy_tuple(const T1&  from, T2& to)
{
    copy_tuple_impl(
            from, to,
            std::make_index_sequence<std::tuple_size<T1>::value>());
}

struct equal_impl{
    template<uint32_t N, uint32_t M>
    static std::true_type _test(char(*)[N == M] = 0);

    template<uint32_t N, uint32_t M>
    static std::false_type _test(char(*)[N != M] = 0);
};

template<uint32_t N, uint32_t M>
struct equal : equal_impl {
    typedef decltype(_test<N, M>()) type;
};