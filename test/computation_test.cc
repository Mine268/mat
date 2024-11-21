#include "mat.hpp"

using namespace matlib;

void equal_test();
void add_sub_test();
void mul_test();

int main() {
    equal_test();
    add_sub_test();
    mul_test();

    return 0;
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
