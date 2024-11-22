#include "mat.hpp"

using namespace matlib;

void equal_test();
void add_sub_test();
void mul_test();
void transpose_test();
void dot_test();

int main() {
    equal_test();
    add_sub_test();
    mul_test();
    transpose_test();
    dot_test();

    return 0;
}

void dot_test() {
    float data1[] = {1, 2, 3, 4};
    float data2[] = {.5, 1, 1.5, 2};
    Matrix<float> mat1 (2, 2, data1);
    Matrix<float> mat2 (2, 2, data2);

    ASSERT_EQ(0.5 * mat1, mat2);
    ASSERT_EQ(mat1 * 0.5, mat2);
}

void transpose_test() {
    float data1[] = {1,2,3,4,5,6,7,8};
    float data2[] = {1,5,2,6,3,7,4,8};
    Matrix<float> mat1 (2, 4, data1);
    Matrix<float> mat2 (4, 2, data2);

    auto mat1_T = mat1.transpose();
    ASSERT_EQ(mat1_T, mat2);
}

void mul_test() {
    float data1[] = {1,2,3,4,5,6};
    float data2[] = {6,4,2,5,3,1};
    Matrix<float> mat1 (2, 3, data1);
    Matrix<float> mat2 (3, 2, data2);

    float data3[] = {19, 17, 52, 47};
    Matrix<float> mat3 (2, 2, data3);

    ASSERT_EQ(mat1 * mat2, mat3);
}

void add_sub_test() {
    float data1[] = {1,2,3,4,5,6};
    float data2[] = {2,3,4,5,6,7};
    Matrix<float> mat1 (2, 3, data1);
    Matrix<float> mat2 (2, 3, data2);

    float data3[] = {3,4,5,6,7,8};
    Matrix<float> mat3 (2, 3, data3);

    float data4[] = {1,1,1,1,1,1};
    Matrix<float> mat4 (2, 3, data4);

    auto mat5 = mat2 - mat1;
    ASSERT_EQ(mat5, mat4);
}

void equal_test() {
    float data1[] = {1,2,3,4,5,6};
    float data2[] = {2,3,4,5,6,7};

    Matrix<float> mat1 (2, 3, data1);
    Matrix<float> mat2 (2, 3, data1);
    Matrix<float> mat3 (2, 3, data2);

    ASSERT_EQ(mat1, mat1);
    ASSERT_EQ(mat1, mat2);
    ASSERT_NE(mat1, mat3);

    Matrix<float> mat4 (2, 2);
    ASSERT_NE(mat4, mat1);
}
