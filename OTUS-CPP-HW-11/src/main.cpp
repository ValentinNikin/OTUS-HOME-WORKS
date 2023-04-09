#include <iostream>

#include "App.h"

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "You should run application with 2 arguments, for example: ./join_server 8080" << std::endl;
        return 1;
    }

    ushort port = static_cast<ushort>(atoi(argv[1]));

    App app(port);
    app.run();

    return 0;
}