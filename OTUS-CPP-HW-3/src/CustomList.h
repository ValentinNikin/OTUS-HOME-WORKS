#pragma once

#include <memory>
#include <type_traits>


/**
 * @brief Пользовательский List.
 *
 * Представляет из себя двунаправленный список.
 * Реализует некоторые простые методы для добавления элементов,
 * а также итераторы.
 *
 * @tparam ITEM_TYPE тип элементов
 * @tparam ALLOCATOR аллокатор
 */
template<typename ITEM_TYPE, typename ALLOCATOR = std::allocator<ITEM_TYPE>>
struct CustomList {
private:

    /**
     * @brief Структура с описанием узла списка
     * @tparam U тип данных, которые хранятся в узле
     */
    template<typename U>
    struct Node {
        U data;
        Node* next = nullptr;
        Node* prev = nullptr;
    };

    using nodeAllocType = typename ALLOCATOR::template rebind<Node<ITEM_TYPE>>::other;

public:

    CustomList();
    ~CustomList();

    /**
     * @brief Добавить в начало списка
     * @param item элемент
     */
    void pushFront(ITEM_TYPE item);

    /**
     * @brief Добавить в конец списка
     * @param item элемент
     */
    void pushBack(ITEM_TYPE item);

    /**
     * @return Количество элементов в списке
     */
    std::size_t size() const;

    /**
     * @brief Удалить все элементы из списка
     */
    void clear();

    /**
     * @brief bidirectional итератор
     */
    struct iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::remove_pointer_t<ITEM_TYPE>;
        using pointer           = value_type*;
        using reference         = value_type&;

        iterator() : _ptr() {}
        explicit iterator(Node<ITEM_TYPE>* ptr) : _ptr(ptr) {}

        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<std::is_pointer<U>::value, bool> = true>
        reference operator*() const {
            return *(_ptr->data);
        }
        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<std::is_pointer<U>::value, bool> = true>
        pointer operator->() const {
            return _ptr->data;
        }

        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<!std::is_pointer<U>::value, bool> = true>
        reference operator*() const {
            return _ptr->data;
        }
        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<!std::is_pointer<U>::value, bool> = true>
        pointer operator->() const {
            return &_ptr->data;
        }

        iterator& operator++() { _ptr = _ptr->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; _ptr = _ptr->next; return tmp; }
        iterator& operator--() { _ptr = _ptr->prev; return *this; }
        iterator operator--(int) { iterator tmp = *this; _ptr = _ptr->prev; return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a._ptr == b._ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a._ptr != b._ptr; };

    private:
        Node<ITEM_TYPE>* _ptr;
    };

    /**
     * @brief bidirectional константный итератор
     */
    struct const_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::remove_pointer_t<ITEM_TYPE>;
        using pointer           = const value_type*;
        using reference         = const value_type&;

        const_iterator() : _ptr() {}
        explicit const_iterator(Node<ITEM_TYPE>* ptr) : _ptr(ptr) {}
        const_iterator(const iterator& iterator) : _ptr(iterator._ptr) {}

        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<std::is_pointer<U>::value, bool> = true>
        reference operator*() const {
            return *(_ptr->data);
        }
        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<std::is_pointer<U>::value, bool> = true>
        pointer operator->() const {
            return _ptr->data;
        }

        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<!std::is_pointer<U>::value, bool> = true>
        reference operator*() const {
            return _ptr->data;
        }
        template<
                typename U = ITEM_TYPE,
                std::enable_if_t<!std::is_pointer<U>::value, bool> = true>
        pointer operator->() const {
            return &_ptr->data;
        }

        const_iterator& operator++() { _ptr = _ptr->next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; _ptr = _ptr->next; return tmp; }
        const_iterator& operator--() { _ptr = _ptr->prev; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; _ptr = _ptr->prev; return tmp; }

        friend bool operator== (const const_iterator& a, const const_iterator& b) { return a._ptr == b._ptr; };
        friend bool operator!= (const const_iterator& a, const const_iterator& b) { return a._ptr != b._ptr; };

    private:
        Node<ITEM_TYPE>* _ptr;
    };

    /**
     * @return итератор начала списка
     */
    iterator begin() { return iterator(_beginPtr); }

    /**
     * @return итератор конца списка
     */
    iterator end() { return iterator(_endPtr); }

    /**
     * @return константный итератор начала списка
     */
    const_iterator cbegin() const { return const_iterator(_beginPtr); }

    /**
     * @return константный итератор конца списка
     */
    const_iterator cend() const { return const_iterator(_endPtr); }

private:
    nodeAllocType _allocator;
    Node<ITEM_TYPE>* _beginPtr = nullptr;
    Node<ITEM_TYPE>* _endPtr = nullptr;
};

template<typename ITEM_TYPE, typename ALLOCATOR>
CustomList<ITEM_TYPE, ALLOCATOR>::CustomList() {
    _allocator = nodeAllocType{};

    _endPtr = _allocator.allocate(1);
    _allocator.construct(_endPtr);
}

template<typename ITEM_TYPE, typename ALLOCATOR>
CustomList<ITEM_TYPE, ALLOCATOR>::~CustomList() {
    clear();

    _allocator.destroy(_endPtr);
    _allocator.deallocate(_endPtr, 1);
}

template<typename ITEM_TYPE, typename ALLOCATOR>
std::size_t CustomList<ITEM_TYPE, ALLOCATOR>::size() const {
    if (_beginPtr == nullptr) return 0;

    auto currentPtr = _beginPtr;
    std::size_t size = 0;

    while (currentPtr != _endPtr) {
        size++;
        currentPtr = currentPtr->next;
    }

    return size;
}

template<typename ITEM_TYPE, typename ALLOCATOR>
void CustomList<ITEM_TYPE, ALLOCATOR>::clear() {
    if (_beginPtr == nullptr) return;

    auto nextPtr= _beginPtr;
    do {
        auto currentPtr = nextPtr;
        nextPtr = currentPtr->next;
        currentPtr->prev = nullptr;
        currentPtr->next = nullptr;

        _allocator.destroy(currentPtr);
        _allocator.deallocate(currentPtr, 1);
    } while (nextPtr != _endPtr);

    _beginPtr = nullptr;
    _endPtr->prev = nullptr;
}

template<typename ITEM_TYPE, typename ALLOCATOR>
void CustomList<ITEM_TYPE, ALLOCATOR>::pushFront(ITEM_TYPE item) {
    auto ptr = _allocator.allocate(1);
    _allocator.construct(ptr);

    ptr->data = item;

    if (_beginPtr == nullptr) {
        _beginPtr = ptr;
        _beginPtr->next = _endPtr;
        _endPtr->prev = _beginPtr;
        return;
    }

    ptr->next = _beginPtr;
    _beginPtr->prev = ptr;
    _beginPtr = ptr;
}

template<typename ITEM_TYPE, typename ALLOCATOR>
void CustomList<ITEM_TYPE, ALLOCATOR>::pushBack(ITEM_TYPE item) {
    auto ptr = _allocator.allocate(1);
    _allocator.construct(ptr);

    ptr->data = item;

    if (_beginPtr == nullptr) {
        _beginPtr = ptr;
        _beginPtr->next = _endPtr;
        _endPtr->prev = _beginPtr;
        return;
    }

    _endPtr->prev->next = ptr;
    ptr->prev = _endPtr->prev;
    ptr->next = _endPtr;
    _endPtr->prev = ptr;
}