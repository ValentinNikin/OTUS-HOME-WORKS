#include <iostream>
#include <map>
#include <algorithm>

#include "CustomAllocator.h"
#include "CustomList.h"

int main (int, char **) {

    // map with default allocator
    {
        std::cout << "Map with default allocator" << std::endl;

        std::map<int, int> map;
        map.emplace(0, 1);

        std::cout << "[" << 0 << "] = " << map[0] << std::endl;

        for (int i = 1; i < 10; i++) {
            map.emplace(i, map[i - 1] * i);
            std::cout << "[" << i << "] = " << map[i] << std::endl;
        }

        std::cout << "---" << std::endl;
    }

    // map with custom allocator
    {
        std::cout << "Map with custom allocator" << std::endl;

        std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>, 10>> mapWithAlloc;
        mapWithAlloc.emplace(0, 1);

        std::cout << "[" << 0 << "] = " << mapWithAlloc[0] << std::endl;

        for (int i = 1; i < 10; i++) {
            mapWithAlloc.emplace(i, mapWithAlloc[i - 1] * i);
            std::cout << "[" << i << "] = " << mapWithAlloc[i] << std::endl;
        }

        std::cout << "---" << std::endl;
    }

    // custom container with default allocator
    {
        std::cout << "Custom container with default allocator" << std::endl;

        CustomList<int> list;

        for (int i = 0; i < 10; i++) {
            list.pushBack(i);
        }

        for (auto it = list.cbegin(); it != list.cend(); it++) {
            std::cout << *it << " ";
        }

        std::cout << std::endl;

        std::cout << "---" << std::endl;
    }

    // custom container with custom allocator
    {
        std::cout << "Custom container with custom allocator" << std::endl;

        CustomList<int, CustomAllocator<int, 10>> listWithAlloc;

        for (int i = 0; i < 10; i++) {
            listWithAlloc.pushBack(i);
        }

        for (auto it = listWithAlloc.cbegin(); it != listWithAlloc.cend(); it++) {
            std::cout << *it << " ";
        }

        std::cout << std::endl;

        std::cout << "---" << std::endl;
    }


    return 0;
}
