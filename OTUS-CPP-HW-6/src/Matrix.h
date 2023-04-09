#pragma once

#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>
#include <stdint.h>

// Исходная реализация для случая двумерной матрицы

template<typename ItemType, ItemType defaultValue>
class Matrix {
private:
    using StorageType = std::map<std::pair<uint32_t, uint32_t>, ItemType>;
public:

    class MatrixValueProxy {
    public:
        operator ItemType();
        void operator=(const ItemType& value);
        ItemType get() const;

        MatrixValueProxy() = delete;

        MatrixValueProxy(const MatrixValueProxy& mrp) = delete;
        MatrixValueProxy& operator=(const MatrixValueProxy& mrp) = delete;

        MatrixValueProxy(MatrixValueProxy&& mrp) = delete;
        MatrixValueProxy& operator=(MatrixValueProxy&& mrp) = delete;
    private:
        friend class Matrix<ItemType, defaultValue>;
        friend class MatrixRowProxy;

        MatrixValueProxy(Matrix<ItemType, defaultValue>& parent, uint32_t row, uint32_t column);

        Matrix<ItemType, defaultValue>& _parent;
        uint32_t _row;
        uint32_t _column;
    };

    class MatrixRowProxy {
    public:
        MatrixValueProxy operator[](uint32_t column);

        MatrixRowProxy() = delete;

        MatrixRowProxy(const MatrixRowProxy& mrp) = delete;
        MatrixRowProxy& operator=(const MatrixRowProxy& mrp) = delete;

        MatrixRowProxy(MatrixRowProxy&& mrp) = delete;
        MatrixRowProxy& operator=(MatrixRowProxy&& mrp) = delete;
    private:
        friend class Matrix<ItemType, defaultValue>;

        MatrixRowProxy(Matrix<ItemType, defaultValue>& parent, uint32_t row);

        Matrix<ItemType, defaultValue>& _parent;
        uint32_t _row;
    };

public:

    class Iterator {
    public:
        using difference_type [[maybe_unused]] = ptrdiff_t;
        using value_type [[maybe_unused]] = std::tuple<uint32_t, uint32_t, ItemType>;
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
            return std::make_tuple(_currentIt->first.first, _currentIt->first.second, _currentIt->second);
        }

    private:
        typename StorageType::iterator _currentIt;
    };

public:

    Matrix() = default;
    ~Matrix() = default;

    MatrixRowProxy operator[](uint32_t row);

    Iterator begin() {
        return Iterator(std::begin(_innerStorage));
    }

    Iterator end() {
        return Iterator(std::end(_innerStorage));
    }

    std::size_t size() const;
    bool empty() const;
private:
    ItemType get(uint32_t row, uint32_t column);
    void set(uint32_t row, uint32_t column, const ItemType& value);

    StorageType _innerStorage;
};

/// Matrix implementation

template<typename ItemType, ItemType defaultValue>
typename Matrix<ItemType, defaultValue>::MatrixRowProxy Matrix<ItemType, defaultValue>::operator[](uint32_t row) {
    return MatrixRowProxy(*this, row);
}

template<typename ItemType, ItemType defaultValue>
void Matrix<ItemType, defaultValue>::set(uint32_t row, uint32_t column, const ItemType& value) {
    if (value == defaultValue) {
        if (!_innerStorage.count({row, column})) return;
        _innerStorage.erase({row, column});
    }
    else {
        _innerStorage[{row, column}] = value;
    }
}

template<typename ItemType, ItemType defaultValue>
ItemType Matrix<ItemType, defaultValue>::get(uint32_t row, uint32_t column) {
    if (!_innerStorage.count({row, column})) return defaultValue;
    return _innerStorage[{row, column}];
}

template<typename ItemType, ItemType defaultValue>
std::size_t Matrix<ItemType, defaultValue>::size() const {
    return _innerStorage.size();
}

template<typename ItemType, ItemType defaultValue>
bool Matrix<ItemType, defaultValue>::empty() const {
    return size() == 0;
}

/// MatrixRowProxy implementation

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixRowProxy::MatrixRowProxy(
        Matrix<ItemType, defaultValue>& parent, uint32_t row)
        : _parent(parent), _row(row) {}

template<typename ItemType, ItemType defaultValue>
typename Matrix<ItemType, defaultValue>::MatrixValueProxy
Matrix<ItemType, defaultValue>::MatrixRowProxy::operator[](uint32_t column) {
    return MatrixValueProxy(_parent, _row, column);
}

/// MatrixValueProxy implementation

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixValueProxy::MatrixValueProxy(
        Matrix<ItemType, defaultValue>& parent, uint32_t row, uint32_t column)
        : _parent(parent), _row(row), _column(column) {}

template<typename ItemType, ItemType defaultValue>
Matrix<ItemType, defaultValue>::MatrixValueProxy::operator ItemType() {
    return get();
}

template<typename ItemType, ItemType defaultValue>
void Matrix<ItemType, defaultValue>::MatrixValueProxy::operator=(const ItemType& value) {
    _parent.set(_row, _column, value);
}

template<typename ItemType, ItemType defaultValue>
ItemType Matrix<ItemType, defaultValue>::MatrixValueProxy::get() const {
    return _parent.get(_row, _column);
}