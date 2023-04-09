#include "gtest/gtest.h"

#include "MatrixN.h"

TEST(TestMatrixN, Matix2) {
    MatrixN<int, -1, 2> matrix2N;

    ASSERT_EQ((int)matrix2N[0][1], -1);
    ASSERT_EQ(matrix2N.size(), 0);

    matrix2N[0][1] = 10;
    ASSERT_EQ((int)matrix2N[0][1], 10);
    ASSERT_EQ(matrix2N.size(), 1);

    matrix2N[10][1] = 5;
    ASSERT_EQ((int)matrix2N[10][1], 5);
    ASSERT_EQ(matrix2N.size(), 2);

    matrix2N[0][1] = -1;
    ASSERT_EQ((int)matrix2N[0][1], -1);
    ASSERT_EQ(matrix2N.size(), 1);

    ((matrix2N[0][1] = -1) = 10) = 20;
    ASSERT_EQ((int)matrix2N[0][1], 20);
    ASSERT_EQ(matrix2N.size(), 2);
}

TEST(TestMatrixN, Matix5) {
    MatrixN<int, 10, 5> matrix5N;

    ASSERT_EQ((int)matrix5N[0][1][2][3][4], 10);
    ASSERT_EQ(matrix5N.size(), 0);

    matrix5N[0][1][2][3][4] = 5;
    ASSERT_EQ((int)matrix5N[0][1][2][3][4], 5);
    ASSERT_EQ(matrix5N.size(), 1);

    matrix5N[0][1][2][3][4] = 10;
    ASSERT_EQ((int)matrix5N[0][1][2][3][4], 10);
    ASSERT_EQ(matrix5N.size(), 0);

    ((matrix5N[0][1][2][3][4] = 10) = 5) = 0;
    ASSERT_EQ((int)matrix5N[0][1][2][3][4], 0);
    ASSERT_EQ(matrix5N.size(), 1);
}