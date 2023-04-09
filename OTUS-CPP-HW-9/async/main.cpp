#include <thread>
#include <functional>

#include "async.h"

void thread1Func(async::handle_t h) {
    async::receive(h, "1\n1\n1\n1\n1\n1\n1\n1\n1\n1", 19);
    async::receive(h, "thread\n1", 8);
}

void thread2Func(async::handle_t h) {
    async::receive(h, "2\n2\n2\n2\n2\n2\n2\n2\n2\n2", 19);
    async::receive(h, "thread\n2", 8);
}

void thread3Func(async::handle_t h) {
    async::receive(h, "{\n3\n3\n3\n3\n3\n3\n3\n3\n}", 19);
    async::receive(h, "thread\n3", 8);
}

void thread4Func(async::handle_t h) {
    async::receive(h, "4\n4\n4\n4\n4\n4\n4\n4\n4\n4", 19);
    async::receive(h, "thread\n4", 8);
}

int main(int, char *[]) {
    {
        // 1. Basic example

        std::size_t blockSize = 5;
        auto h = async::connect(blockSize);
        auto h2 = async::connect(blockSize);
        async::receive(h, "1", 1);
        async::receive(h2, "1\n", 2);
        async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
        async::receive(h, "b\nc\nd\n}\n89\n", 11);
        async::disconnect(h);
        async::disconnect(h2);
    }


    {
        // 2. Example with threads

        auto h1 = async::connect(3);
        auto h2 = async::connect(5);

        std::thread t1(&thread1Func, std::ref(h1));
        std::thread t2(&thread2Func, std::ref(h2));
        std::thread t3(&thread3Func, std::ref(h1));
        std::thread t4(&thread4Func, std::ref(h2));

        if (t1.joinable()) {
            t1.join();
        }

        if (t2.joinable()) {
            t2.join();
        }

        if (t3.joinable()) {
            t3.join();
        }

        if (t4.joinable()) {
            t4.join();
        }

        async::disconnect(h1);
        async::disconnect(h2);
    }

    return 0;
}