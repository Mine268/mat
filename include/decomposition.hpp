#pragma once

// std
#include <tuple>
#include <numeric>
// matlib
#include "utility.hpp"
#include "common.hpp"


namespace matlib {

template<typename> class Matrix;

template<typename V>
std::tuple<Matrix<V>, Matrix<V>, Matrix<V>>
PLU_decomposite(const Matrix<V> &mat) {
    auto origin_shape = mat.get_shape();
    Matrix<V> inplace = mat;
    // line rearange
    std::vector<Size_T> permute (origin_shape.first);
    std::iota(permute.begin(), permute.end(), 0);
    // prepare for loop
    Size_T i{}, j{};
    while (j < origin_shape.second) {
        // find largest row index in columns
        Size_T iM = origin_shape.first + 1;
        V largest = Default<V>::zero;
        for (Size_T k = i; k < origin_shape.first; ++k) {
            if (std::abs(inplace.at(k, j)) > largest) {
                largest = std::abs(inplace.at(k, j));
                iM = k;
            }
        }
        // all zero column
        if (iM == origin_shape.first + 1) {
            ++j;
            continue;
        }
        // swap largest to current
        std::swap(permute[i], permute[iM]);
        for (Size_T k = 0; k < origin_shape.second; ++k) {
            std::swap(inplace.at(i, k), inplace.at(iM, k));
        }
        // process each row below
        for (Size_T k = i + 1; k < origin_shape.first; ++k) {
            V t = inplace.at(k, j) / inplace.at(i, j);
            for (Size_T s = j + 1; s < origin_shape.second; ++s) {
                inplace.at(k, s) = inplace.at(k, s) - t * inplace.at(i, s);
            }
            inplace.at(k, j) = t;
        }
        ++j, ++i;
    }
    // process output
    Matrix<V> P_mat(origin_shape.first, origin_shape.first),
        L_mat(origin_shape.first, origin_shape.first),
        U_mat(origin_shape);
    for (Size_T i = 0; i < origin_shape.first; ++i) {
        for (Size_T j = 1; j <= i; ++j) {
            L_mat.at(i, j - 1) = inplace.at(i, j - 1);
        }
        L_mat.at(i, i) = 1;
    }
    for (Size_T i = 0; i < origin_shape.first; ++i) {
        for (Size_T j = i; j < origin_shape.second; ++j) {
            U_mat.at(i, j) = inplace.at(i, j);
        }
    }
    for (Size_T k = 0; k < origin_shape.first; ++k) {
        P_mat.at(k, permute[k]) = Default<V>::one;
    }
    return {P_mat, L_mat, U_mat};
}

}