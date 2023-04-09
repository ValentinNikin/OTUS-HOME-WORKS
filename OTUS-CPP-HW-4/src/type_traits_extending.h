#pragma once

#include <type_traits>
#include <vector>
#include <list>

/**
 * Содержит вспомогательные функции для определения типов
 */
struct type_traits_extending {
public:
    template<typename T>
    using remove_cvref_t
            = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
private:

    /// or
    template<typename T1, typename T2>
    struct _or : std::conditional<T1::value, T1, T2>::type {};

    ///@{
    /// Help functions to determine the type is vector or list

    struct _help_is_vector_or_list_impl {
        template<
                typename T,
                typename ItemType = typename T::value_type,
                typename AllocatorType = typename T::allocator_type,
                typename = std::enable_if_t<
                        _or<
                                typename std::is_same<T, std::vector<ItemType, AllocatorType>>::type,
                                typename std::is_same<T, std::list<ItemType, AllocatorType>>::type
                        >::value,
                        void>
        >
        static std::true_type _test(int);

        template<typename>
        static std::false_type _test(...);
    };

    template<typename T>
    struct _is_vector_or_list_impl : _help_is_vector_or_list_impl {
        typedef decltype(_test<T>(0)) type;
    };

    ///}

    ///@{
    /// Help functions to determine the type is tuple

    struct _help_is_same_variadic_type {
        template<
                typename T,
                typename... Types,
                typename = std::enable_if_t<(std::is_same<remove_cvref_t<T>, remove_cvref_t<Types>>::value && ...), void>
        >
        static std::true_type _test(int);

        template<typename>
        static std::false_type _test(...);
    };

    template<typename... T>
    struct _is_same_variadic_type_impl : _help_is_same_variadic_type {
        typedef decltype(_test<T...>(0)) type;
    };

    template<typename>
    struct _is_tuple_like_impl : std::false_type { };

    template<typename T, typename... Types>
    struct _is_tuple_like_impl<std::tuple<T, Types...>> : _is_same_variadic_type_impl<T, Types...>::type {};

    ///}
public:
    /// Определить, что тип является std::vector или std::list
    template<typename T>
    struct is_vector_or_list : _is_vector_or_list_impl<remove_cvref_t<T>>::type {};

    /// Определить, что тип является std::string
    template<typename T>
    struct is_string : std::is_same<remove_cvref_t<T>, std::basic_string<char>>::type {};

    /// Определить, что тип является std::tuple, и все типы элементов, которые хранятся в tuple совпадают
    template<typename T>
    struct is_tuple : _is_tuple_like_impl<remove_cvref_t<T>>::type {};
};
