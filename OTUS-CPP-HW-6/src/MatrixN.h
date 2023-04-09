#pragma once

#include <map>

#include "./help_types.h"

/**
 * Хранилище (N-мерный ключ - значение), которое предоставляет
 * доступ к элементам используя интерфейс N-мерной матрицы.
 * @tparam ItemType тип элементов
 * @tparam defaultValue значение по умолчанию
 * @tparam N размерность
 */
template<typename ItemType, ItemType defaultValue, uint32_t N>
class MatrixN {
    using StorageKey = tuple_N<uint32_t, N>;
    using StorageType = std::map<StorageKey, ItemType>;
public:

    /**
     * Proxy класс для взаимодействия с элементами матрицы.
     * Реализует оператор приведения к ItemType и
     * каноническую форму оператора присваивания.
     */
    class MatrixProxyValue {
        using ValueKey = tuple_N<uint32_t, N>;
    public:
        MatrixProxyValue(MatrixN<ItemType, defaultValue, N>& storage, ValueKey& key)
            : _storage(storage), _key(key) {}

        operator ItemType() {
            return _storage.get(_key);
        }

        MatrixProxyValue& operator=(const ItemType& value) {
            _storage.set(_key, value);
            return *this;
        }

    private:
        MatrixN<ItemType, defaultValue, N>& _storage;
        ValueKey _key;
    };

    /**
     * Proxy класс строки матрицы
     * @tparam M текущий уровень вложенности
     */
    template<uint32_t M>
    class MatrixRowProxy {
        using RowProxyKey = tuple_N<uint32_t, M>;
    public:

        MatrixRowProxy(MatrixN<ItemType, defaultValue, N>& storage, RowProxyKey& key)
            : _storage(storage), _key(key) {}

        /**
         * Перегрузка оператора[] для случая, когда элемент матрицы
         * еще не достигнут (находимся на M-уровне вложенности, M + 1 < N)
         */
        template<uint32_t K = N, uint32_t L = M + 1,
                std::enable_if_t<!equal<K, L>::type::value, bool> = true>
        MatrixRowProxy<M + 1> operator[](uint32_t index) {
            tuple_N<uint32_t, M + 1> newKey;
            copy_tuple(_key, newKey);
            std::get<M>(newKey) = index;
            return MatrixRowProxy<M + 1>(_storage, newKey);
        }

        /**
         * Перегрузка оператора [] для случая, когда достигнут
         * элемент матрицы (M + 1 == N)
         */
        template<uint32_t K = N, uint32_t L = M + 1,
                std::enable_if_t<equal<K, L>::type::value, bool> = true>
        MatrixProxyValue operator[](uint32_t index) {
            tuple_N<uint32_t, M + 1> newKey;
            copy_tuple(_key, newKey);
            std::get<M>(newKey) = index;
            return MatrixProxyValue(_storage, newKey);
        }

    private:
        MatrixN<ItemType, defaultValue, N>& _storage;
        RowProxyKey _key;
    };

    /**
     * Итератор для обхода элементов матрицы
     */
    class Iterator {
    public:
        using difference_type [[maybe_unused]] = ptrdiff_t;
        using value_type [[maybe_unused]] = tuple_N<uint32_t, N + 1>;
        using iterator_category [[maybe_unused]] = std::forward_iterator_tag;

        Iterator(typename StorageType::iterator&& it)
                : _currentIt(it) {}

        Iterator& operator++() {
            _currentIt++;
            return *this;
        }

        bool operator==(Iterator other) const {
            return _currentIt == other._currentIt;
        }

        bool operator!=(Iterator other) const {
            return !(*this == other);
        }

        value_type operator*() {
            value_type retValue;
            copy_tuple(_currentIt->first, retValue);
            std::get<N>(retValue) = _currentIt->second;
            return retValue;
        }

    private:
        typename StorageType::iterator _currentIt;
    };

public:

    MatrixRowProxy<1> operator[](uint32_t index) {
        auto tuple = std::tuple<uint32_t>(index);
        return MatrixRowProxy<1>(*this, tuple);
    }

    Iterator begin() {
        return Iterator(std::begin(_storage));
    }

    Iterator end() {
        return Iterator(std::end(_storage));
    }

    std::size_t size() const {
        return _storage.size();
    }

private:

    /**
     * Получить элемент матрицы.
     * @param key ключ элемента
     * @return элемент с ключом key, если таковой существует,
     * defaultValue в противном случае.
     */
    ItemType get(StorageKey key) const {
        auto item = _storage.find(key);
        if (item != _storage.end()) {
            return item->second;
        }
        return defaultValue;
    }

    /**
     * Установить элемент матрицы с ключом key в значение value.
     * Если value == defaultValue, то элемент будет удален из хранилища.
     * @param key ключ элемента
     * @param value значение
     */
    void set(StorageKey key, ItemType value) {
        if (value == defaultValue) {
            auto item = _storage.find(key);
            if (item != _storage.end()) {
                _storage.erase(item);
            }
            return;
        }
        _storage[key] = value;
    }

    StorageType _storage;
};