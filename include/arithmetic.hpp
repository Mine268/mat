#include "utility.hpp"
#include "common.hpp"

namespace matlib {

template<typename> class Matrix;

/// basic operations
template<
    typename V,
    typename U>
bool operator==(const Matrix<V> &lhs, const Matrix<U> &rhs) {
    if (&lhs == &rhs) {
        return true;
    }

    if (lhs.get_shape() != rhs.get_shape()) {
        return false;
    }

    auto shape = lhs.get_shape();
    auto tot = shape.first * shape.second;

    auto p_lhs = lhs.raw_data();
    auto p_rhs = rhs.raw_data();
    for (Size_T i = 0; i < tot; ++i) {
        if (!ValueEq(p_lhs[i], p_rhs[i])) {
            return false;
        }
    }

    return true;
}

template<
    typename V,
    typename U>
bool operator!=(const Matrix<V> &lhs, const Matrix<U> &rhs) {
    return !(lhs == rhs);
}

template<
    typename V,
    typename U>
auto operator+(const Matrix<V> &lhs, const Matrix<U> &rhs) {
    ASSERT_MSG(lhs.get_shape() == rhs.get_shape(), "Shape must match for addition.");
    Matrix<decltype(V{} + U{})> retval(lhs.get_shape());
    auto shape = lhs.get_shape();
    auto tot = shape.first * shape.second;

    auto p_lhs = lhs.raw_data();
    auto p_rhs = rhs.raw_data();
    auto p_ret = retval.raw_data();

    for (Size_T i = 0; i < tot; ++i) {
        p_ret[i] = p_lhs[i] + p_rhs[i];
    }

    return retval;
}

template<
    typename V,
    typename U>
auto operator-(const Matrix<V> &lhs, const Matrix<U> &rhs) {
    ASSERT_MSG(lhs.get_shape() == rhs.get_shape(), "Shape must match for subtraction.");
    Matrix<decltype(V{} + U{})> retval(lhs.get_shape());
    auto shape = lhs.get_shape();
    auto tot = shape.first * shape.second;

    auto p_lhs = lhs.raw_data();
    auto p_rhs = rhs.raw_data();
    auto p_ret = retval.raw_data();

    for (Size_T i = 0; i < tot; ++i) {
        p_ret[i] = p_lhs[i] - p_rhs[i];
    }

    return retval;
}

template<
    typename V,
    typename U>
auto operator*(U k, const Matrix<V> &rhs) {
    auto shape = rhs.get_shape();
    Matrix<V> retval (shape);

    auto target_it = retval.raw_begin();
    auto source_it = rhs.raw_const_begin();

    for (; target_it != retval.raw_end(); ++target_it, ++source_it) {
        *target_it = k * *source_it;
    }
    return retval;
}

template<
    typename V,
    typename U>
auto operator*(const Matrix<V> &rhs, U k) {
    auto shape = rhs.get_shape();
    Matrix<V> retval (shape);

    auto target_it = retval.raw_begin();
    auto source_it = rhs.raw_const_begin();

    for (; target_it != retval.raw_end(); ++target_it, ++source_it) {
        *target_it = k * *source_it;
    }
    return retval;
}

// TODO: directly index on raw array to accelerate
template<
    typename V,
    typename U>
auto operator*(const Matrix<V> &lhs, const Matrix<U> &rhs) {
    using Return_T = decltype(decltype(V{} + U{}){} * decltype(V{} + U{}){});
    ASSERT_MSG(lhs.get_shape().second == rhs.get_shape().first,
        "Shape must match for multiplication.");
    
    auto lshape = lhs.get_shape(); 
    auto rshape = rhs.get_shape();

    Matrix<Return_T> retval(lshape.first, rshape.second);
    for (Size_T r = 0; r < lshape.first; ++r) {
        for (Size_T c = 0; c < rshape.second; ++c) {
            Return_T cur{};
            for (Size_T k = 0; k < lshape.second; ++k) {
                cur = cur + lhs.unsafe_at(r, k) * rhs.unsafe_at(k, c);
            }
            retval.unsafe_at(r, c) = cur;
        }
    }

    return retval;
}

}