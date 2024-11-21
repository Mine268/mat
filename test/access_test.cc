// std
#include <utility>
// matlib
#include "mat.hpp"


using namespace matlib;

void create_test();
void access_test();

int main() {
    create_test();
    access_test();

    return 0;
}

void create_test() {
    Matrix<float> mat1(3, 4);
    Matrix<float> mat2(std::make_pair(4, 3));

    float raw_data[] = {1, 2, 3, 4};
    Matrix<float> mat3(2, 2, raw_data);
    Matrix<float> mat4(std::make_pair(1, 4), raw_data);
}

void access_test() {
    float raw_data[] = {1, 2, 3, 4};
    Matrix<float> mat1(2, 2, raw_data);
    const Matrix<float> mat2(std::make_pair(1, 4), raw_data);

    ASSERT_EQ(mat1.at(0, 0), mat2.at(0, 0));
    ASSERT_EQ(mat1.at(0, 1), mat2.at(0, 1));
    ASSERT_EQ(mat1.at(1, 0), mat2.at(0, 2));
    ASSERT_EQ(mat1.at(1, 1), mat2.at(0, 3));

    mat1.at(1, 1) = 233.f;
    ASSERT_NE(mat1.at(1, 1), mat2.at(0, 3));

    mat1.at(1, 1) = 4;
    ASSERT_EQ(mat1.at(1, 1), mat2.at(0, 3));
}
