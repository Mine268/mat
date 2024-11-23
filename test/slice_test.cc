#include "mat.hpp"

using namespace matlib;

void slice_test();
void sub_slice_test();
void slice_to_mat_test();

int main() {
    slice_test();
    sub_slice_test();
    slice_to_mat_test();
    return 0;
}

void slice_to_mat_test() {
    float data[] = {1,2,3,4,5,6,7,8,9};
    const Matrix<float> mat (3, 3, data);

    auto slice1 = mat.slice(0, 1, 1, 2);
    auto slice2 = slice1.slice(1, 1, 0, 0);

    auto new_mat = slice2.to_mat();

    ASSERT_EQ(new_mat.get_shape().first, 1);
    ASSERT_EQ(new_mat.get_shape().second, 1);
    ASSERT_EQ(new_mat.at(0, 0), 5);
}

void sub_slice_test() {
    float data[] = {1,2,3,4,5,6,7,8,9};
    const Matrix<float> mat (3, 3, data);

    auto slice1 = mat.slice(0, 1, 1, 2);
    auto slice2 = slice1.slice(1, 1, 0, 0);

    ASSERT_EQ(slice2.at(0, 0), 5);
}

void slice_test() {
    float data[] = {1,2,3,4,5,6,7,8,9};
    Matrix<float> mat (3, 3, data);
    auto slice = mat.slice(0, 1, 1, 2);
    auto &ele = slice.at(1, 1);
    ele = 2333;

    ASSERT_EQ(mat.at(1, 2), 2333);
}
