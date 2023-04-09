
#include <iostream>

#include "MatrixN.h"

int main (int, char **) {
    MatrixN<int, 0, 2> matrix;

    for (int i = 0; i <= 9; i++) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Busy items count: " << matrix.size() << std::endl;

    std::cout << "Busy items: " << std::endl;
    for (auto item: matrix) {
        uint32_t x;
        uint32_t y;
        uint32_t value;

        std::tie(x, y, value) = item;

        std::cout << "[" << x << ", " << y << "] = " << value << std::endl;
    }

    // каноническая форма оператора "="
    (((matrix[1][1] = 10) = 20) = 30) = 70;

    // 5-мерная матрица
    MatrixN<uint32_t, 0, 5> matrix5N;
    matrix5N[0][1][2][3][4] = 20;

    return 0;
}
