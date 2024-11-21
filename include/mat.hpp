#pragma once

// std
#include <optional>
#include <utility>
#include <vector>

// matlib
#include "common.hpp"
#include "utility.hpp"

namespace matlib {

template<typename V>
class Matrix {
// methods
public:
    // construct
    explicit Matrix(const std::pair<Size_T, Size_T> &, V* = nullptr);
    Matrix(Size_T NR, Size_T NC, V* = nullptr);
    // access
    std::pair<Size_T, Size_T> get_shape() const;
    V* raw_data();
    const V* raw_data() const;
    V& unsafe_at(Size_T, Size_T);
    const V& unsafe_at(Size_T, Size_T) const;
    V& at(Size_T, Size_T);
    const V& at(Size_T, Size_T) const;

// internal
private:
    std::optional<Size_T> offset(Size_T, Size_T) const;

// members
private:
    std::vector<V> data;
    std::pair<Size_T, Size_T> shape;
};

template<typename V>
Matrix<V>::Matrix(const std::pair<Size_T, Size_T> &shape, V* raw_data)
    : data(std::vector<V> {0}), shape(shape)
{
    ASSERT_MSG(!MultiplyOverflow(shape.first, shape.second), "Bad shape.");
    if (nullptr == raw_data) {
        this->data = std::vector<V> (shape.first * shape.second, V{});
    } else {
        this->data = std::vector<V> (raw_data, raw_data + shape.first * shape.second);
    }
}

template<typename V>
Matrix<V>::Matrix(Size_T NR, Size_T NC, V* raw_data) {
    ASSERT_MSG(!MultiplyOverflow(NR, NC), "Bad shape.");
    this->shape = std::make_pair(NR, NC);
    if (nullptr == raw_data) {
        this->data = std::vector<V> (shape.first * shape.second, V{});
    } else {
        this->data = std::vector<V> (raw_data, raw_data + shape.first * shape.second);
    }
}

template<typename V>
std::pair<Size_T, Size_T> Matrix<V>::get_shape() const {
    return shape;
}

template<typename V>
V* Matrix<V>::raw_data() {
    return data.data();
}

template<typename V>
const V* Matrix<V>::raw_data() const {
    return data.data();
}

template<typename V>
std::optional<Size_T> Matrix<V>::offset(Size_T r, Size_T c) const {
    bool valid = r < shape.first && c < shape.second;
    if (valid) {
        return std::make_optional(r * shape.second + c);
    } else {
        return std::nullopt;
    }
}

template<typename V>
V& Matrix<V>::unsafe_at(Size_T r, Size_T c) {
    auto _offset = r * shape.second + c;
    return data.at(_offset);
}

template<typename V>
const V& Matrix<V>::unsafe_at(Size_T r, Size_T c) const {
    auto _offset = r * shape.second + c;
    return data.at(_offset);
}

template<typename V>
V& Matrix<V>::at(Size_T r, Size_T c) {
    auto _offset = offset(r, c);
    ASSERT_MSG(_offset, "Bad indices.");
    return data.at(_offset.value());
}

template<typename V>
const V& Matrix<V>::at(Size_T r, Size_T c) const {
    auto _offset = offset(r, c);
    ASSERT_MSG(_offset, "Bad indices.");
    return data.at(_offset.value());  // implicit convert to const ref
}

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
        if (p_lhs[i] != p_rhs[i]) {
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
