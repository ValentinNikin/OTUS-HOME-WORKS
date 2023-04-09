#pragma once

#include <string>
#include <memory>
#include <future>

#include "map-reducer/include/IMapper.h"
#include "map-reducer/include/IReducer.h"

class ReducerTask {
public:
    ReducerTask(
            const std::string& inFilePath,
            const std::string& outFilePath,
            std::unique_ptr<IReducer> reducer,
            std::unique_ptr<IMapper> mapper);

    void run();
    std::string getResult();
private:
    std::string _inFilePath;
    std::string _outFilePath;
    std::unique_ptr<IReducer> _reducer;
    std::unique_ptr<IMapper> _mapper;

    std::future<std::string> _future;
};