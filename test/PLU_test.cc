#include "mat.hpp"
#include "decomposition.hpp"
#include "utility.hpp"
#include <stdatomic.h>


using namespace matlib;

void PLU_test1() {
    float data[] = {3,-1,2,6,-1,5,9,7,3};
    Matrix<float> mat (3, 3, data);
    auto [P_mat, L_mat, U_mat] = \
        PLU_decomposite(mat);
    
    auto recon = L_mat * U_mat;
    auto PA = P_mat * mat;

    ASSERT_EQ(recon, PA);
}

void PLU_test2() {
    float data[] = {3,4,2,5,78,
        3,1,-4.,4,-39,
        -12,6};
    Matrix<float> A(3, 4, data);
    auto [P, L, U] = \
        PLU_decomposite(A);
    
    auto PA = P * A;
    auto LU = L * U;

    ASSERT_EQ(PA, LU);
}

void PLU_test3() {
    float data[] = {
        1,2,3,4,
        8,7,6,5,
        -1,-1,-1,-1};
    Matrix<float> A (3, 4, data);
    auto [P, L, U] = \
        PLU_decomposite(A);
    
    auto PA = P * A;
    auto LU = L * U;

    ASSERT_EQ(PA, LU);
}

int main() {
    PLU_test1();
    PLU_test2();
    PLU_test3();
    return 0;
}
