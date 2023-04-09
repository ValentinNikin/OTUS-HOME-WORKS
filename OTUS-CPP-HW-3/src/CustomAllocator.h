#pragma once

#include <iostream>

#include "MemoryBlock.h"

// #define PRINT_LOG

/**
 * @brief Пользовательский аллокатор.
 *
 * Позволяет заранее резервировать память для заданного количества объектов
 *
 * @tparam T тип объекта
 * @tparam ALLOCATION_SIZE количество объектов
 */
template <typename T, std::size_t ALLOCATION_SIZE>
struct CustomAllocator {
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind {
        using other = CustomAllocator<U, ALLOCATION_SIZE>;
    };

    CustomAllocator() = default;
    ~CustomAllocator();

    template <typename U>
    CustomAllocator(const CustomAllocator<U, ALLOCATION_SIZE> &) {}

    T *allocate(std::size_t n) {
#ifdef PRINT_LOG
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif

        auto p = allocateMemory(n);
        if (!p)
            throw std::bad_alloc();
        return p;
    }

    void deallocate(T *p, std::size_t n) {
#ifdef PRINT_LOG
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        freeMemory(p, n);
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args) {
#ifdef PRINT_LOG
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
#endif
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p) {
#ifdef PRINT_LOG
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
#endif
        p->~U();
    }

private:

    T* allocateMemory(std::size_t needSize);
    void freeMemory(T* ptr, std::size_t size);

    MemoryBlock<T>** _memoryBlocks {nullptr};
    std::size_t _blocksCount {0};
};

template <typename T, std::size_t ALLOCATION_SIZE>
T* CustomAllocator<T, ALLOCATION_SIZE>::allocateMemory(std::size_t needSize) {
    T* ptr = nullptr;
    for (std::size_t i = 0; i < _blocksCount; i++) {
        ptr = _memoryBlocks[i]->allocateRange(needSize);
        if (ptr != nullptr) {
            break;
        }
    }

    if (ptr != nullptr) {
        return ptr;
    }

    _blocksCount++;
    _memoryBlocks = static_cast<MemoryBlock<T>**>(realloc(_memoryBlocks, sizeof(MemoryBlock<T>*) * _blocksCount));

    _memoryBlocks[_blocksCount - 1] = new MemoryBlock<T>(std::max(ALLOCATION_SIZE, needSize));

    return _memoryBlocks[_blocksCount - 1]->allocateRange(needSize);
}

template <typename T, std::size_t ALLOCATION_SIZE>
void CustomAllocator<T, ALLOCATION_SIZE>::freeMemory(T* ptr, std::size_t size) {
    for (std::size_t i = 0; i < _blocksCount; i++) {
        if (_memoryBlocks[i]->freeRange(ptr, size)) return;
    }
}

template <typename T, std::size_t ALLOCATION_SIZE>
CustomAllocator<T, ALLOCATION_SIZE>::~CustomAllocator() {
    for (std::size_t i = 0; i < _blocksCount; i++) {
        delete _memoryBlocks[i];
    }

    std::free(_memoryBlocks);
}