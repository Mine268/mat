#include <iostream>
#include "mat.hpp"

using namespace matlib;

void slice_test();

int main() {
    slice_test();
    return 0;
}

void slice_test() {
    float data[] = {1,2,3,4,5,6,7,8,9};
    const Matrix<float> mat (3, 3, data);
    auto slice = mat.slice(0, 1, 1, 2);
    auto &ele = slice.at(1, 1);
    return;
}
