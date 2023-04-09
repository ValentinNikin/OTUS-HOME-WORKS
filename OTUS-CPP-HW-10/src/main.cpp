#include <iostream>

#include "App.h"

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "You should run application with 2 arguments, for example: ./bulk_server 8080 3" << std::endl;
        return 1;
    }

    ushort port = static_cast<ushort>(atoi(argv[1]));
    std::size_t blockSize = static_cast<std::size_t>(atoll(argv[2]));

    App app(port, blockSize);
    app.run();

    return 0;
}