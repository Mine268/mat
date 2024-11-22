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
    Matrix<float> mat (3, 3, data);
    auto slice = mat.slice(0, 1, 1, 2);
    auto &ele = slice.at(1, 1);
    ele = 2333;

    for (Size_T r = 0; r < 3; ++r) {
        for (Size_T c = 0; c < 3; ++c) {
            std::cout << mat.at(r, c) << ' ';
        }
        std::cout << std::endl;
    }

    return;
}
