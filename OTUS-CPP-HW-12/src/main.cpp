#include <iostream>
#include <memory>
#include <fstream>

#include "map-reducer/include/MapReducer.h"

#include "UserMapper.h"
#include "UserReducer.h"

bool reducerResultIsOk(const IReducer* reducer, const std::string& resFilePath) {
    std::ifstream file(resFilePath, std::ios_base::in);
    std::string line;
    while (!file.eof()) {
        std::getline(file, line);
        if (!line.empty()) {
            // десериализуем результаты из reducer-а
            auto reducerResult = reducer->makeInitialResult();
            reducerResult->deserialize(line);

            auto prefixes = dynamic_cast<UserReducerResult*>(reducerResult.get())->getPrefixes();
            // проверяем, что каждый из префиксов уникален
            if (!std::all_of(
                    prefixes.cbegin(), prefixes.cend(),
                    [&prefixes](const auto& item) {
                        if (item.isFullWord) return true; // пропускаем, если префикс это полное слово

                        if (item.count == 1) {
                            // проверяем, что найденный префикс не совпадает ни с одним из "полных" слов
                            return !std::any_of(
                                    prefixes.cbegin(), prefixes.cend(),
                                    [prefix = item.prefix](const auto& innerItem) {
                                        return innerItem.isFullWord && innerItem.prefix == prefix;
                                    });
                        }
                        return false;
                    })
                    ) {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cerr << "You should run application with 3 arguments, for example: ./mapreduce src mnum rnum" << std::endl;
        return 1;
    }

    std::string srcFile = argv[1];
    std::size_t mnum = static_cast<std::size_t>(atoi(argv[2]));
    std::size_t rnum = static_cast<std::size_t>(atoi(argv[3]));

    MapReducer mapReducer(mnum, rnum);

    /**
     * Каждому из потоков MapReducer-а нужен свой экземпляр Mapper-а и Reducer-а.
     * Чтобы избежать работы с template-ами, отдаю в MapReducer лямбды,
     * которые позволяют генерировать соответствующие объекты.
     */
    auto reducerGenerator = []() { return std::make_unique<UserReducer>(); };
    mapReducer.setReducerGenerator(reducerGenerator);

    std::size_t minPrefixSize = 0;
    auto mapperGenerator = [&minPrefixSize]() { return std::make_unique<UserMapper>(minPrefixSize); };
    mapReducer.setMapperGenerator(mapperGenerator);

    // reducer "знает" каким образом десериализовать свой результат,
    // именно для этого создаем дополнительный объект reducer-а.
    auto reducer = reducerGenerator();
    bool taskSolved = false;

    do {
        minPrefixSize++;
        auto results = mapReducer.run(srcFile, "./");
        taskSolved = std::all_of(results.cbegin(), results.cend(), [r = reducer.get()](const auto& resFilePath) {
            return reducerResultIsOk(r, resFilePath);
        });
    }
    while (!taskSolved);

    std::cout << "Found min prefix size: " << minPrefixSize << std::endl;

    return 0;
}