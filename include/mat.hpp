#pragma once

// std
#include <functional>
#include <optional>
#include <utility>
#include <vector>
#include <type_traits>

// matlib
#include "common.hpp"
#include "utility.hpp"

namespace matlib {

template<typename> class Matrix;
template<typename, typename > class Slice;

/// Matrix
template<typename V>
class Matrix {
// members
public:
    using RawIterator = typename std::vector<V>::iterator;
    using ConstRawIterator = typename std::vector<V>::const_iterator;
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
    // iterator
    RawIterator raw_begin();
    RawIterator raw_end();
    ConstRawIterator raw_const_begin() const;
    ConstRawIterator raw_const_end() const;
    // slice
    Slice<V, Matrix<V>> slice(Size_T, Size_T, Size_T, Size_T);
    Slice<V, const Matrix<V>> slice(Size_T, Size_T, Size_T, Size_T) const;
    // matrix operation
    Matrix transpose() const;

// internal
private:
    std::optional<Size_T> offset(Size_T, Size_T) const;

private:
    std::vector<V> data;
    std::pair<Size_T, Size_T> shape;
};

/// Range
struct Range {
    Size_T a{}, b{};
    Range(Size_T a_ = 0, Size_T b_ = 0) {
        a = std::min(a_, b_);
        b = std::max(a_, b_);
    }
    bool empty() const {
        return a <= b;
    }
    bool include(const Range& rhs) {
        return a <= rhs.a && b >= rhs.b;
    }
};

/// Slice
template<
    typename V,
    typename MatType>
class Slice {
    using ElementTypeRef = std::conditional_t<std::is_const_v<MatType>,
        const V&,
        V&>;
    using SubSliceType = std::conditional_t<std::is_const_v<MatType>,
        Slice<V, const Matrix<V>>,
        Slice<V, Matrix<V>>>;
    friend class Matrix<V>;
// function
public:
    ElementTypeRef at(Size_T, Size_T);
    ElementTypeRef unsafe_at(Size_T, Size_T);
    SubSliceType slice(Size_T, Size_T, Size_T, Size_T);
    Matrix<V> to_mat();
private:
    explicit Slice(Range, Range, MatType &);

// member
private:
    Range rowRange, colRange;
    std::reference_wrapper<MatType> source;
};


/// ======================================
/// implementation

/// Slice
template<typename V, typename MatType>
typename Slice<V, MatType>::ElementTypeRef Slice<V, MatType>::at(Size_T i, Size_T j) {
    // ASSERT_MSG(source, "Invalid reference to source matrix.");
    ASSERT_MSG(i <= rowRange.b - rowRange.a && j <= colRange.b - colRange.a,
        "Bad indices out of slice.");
    auto r = i + rowRange.a;
    auto c = j + colRange.a;
    return source.get().unsafe_at(r, c);
}

template<typename V, typename MatType>
typename Slice<V, MatType>::ElementTypeRef Slice<V, MatType>::unsafe_at(Size_T i, Size_T j) {
    auto r = i + rowRange.a;
    auto c = j + colRange.a;
    return source.get().unsafe_at(r, c);
}

template<typename V, typename MatType>
typename Slice<V, MatType>::SubSliceType
Slice<V, MatType>::slice(Size_T ra, Size_T rb, Size_T ca, Size_T cb) {
    Range newRowRange {ra, rb};
    Range newColRange {ca, cb};
    // ASSERT_MSG(source, "Invalid reference to source matrix");
    ASSERT_MSG(newRowRange.b <= rowRange.b - rowRange.a
        && newColRange.b <= colRange.b - colRange.a,
        "Sub-slice exceeds the origin one.");
    return typename Slice<V, MatType>::SubSliceType (
        {rowRange.a + newRowRange.a, rowRange.a + newRowRange.b},
        {colRange.a + newColRange.a, colRange.a + newColRange.b},
        source
    );
}

template<typename V, typename MatType>
Matrix<V> Slice<V, MatType>::to_mat() {
    Matrix<V> retval (
        rowRange.b - rowRange.a + 1,
        colRange.b - colRange.a + 1);
    const auto &origin_mat = this->source.get();
    for (Size_T i = rowRange.a; i <= rowRange.b; ++i) {
        for (Size_T j = colRange.a; j <= colRange.b; ++j) {
            retval.unsafe_at(i - rowRange.a, j - colRange.a) = origin_mat.unsafe_at(i, j);
        }
    }
    return retval;
}

template<typename V, typename MatType>
Slice<V, MatType>::Slice(Range rowRange_, Range colRange_, MatType &mat)
    : source(mat)
{
    rowRange = rowRange_;
    colRange = colRange_;
}

/// Matrix

template<typename V>
Slice<V, Matrix<V>> Matrix<V>::slice(Size_T ra, Size_T rb, Size_T ca, Size_T cb) {
    Range rowRange {ra, rb};
    Range colRange {ca, cb};
    ASSERT_MSG(rowRange.b < shape.first && colRange.b < shape.second,
        "Slice output range of matrix.");
    return Slice<V, Matrix<V>> (rowRange, colRange, *this);
}

template<typename V>
Slice<V, const Matrix<V>> Matrix<V>::slice(Size_T ra, Size_T rb, Size_T ca, Size_T cb) const {
    Range rowRange {ra, rb};
    Range colRange {ca, cb};
    ASSERT_MSG(rowRange.b < shape.first && colRange.b < shape.second,
        "Slice output range of matrix.");
    return Slice<V, const Matrix<V>> (rowRange, colRange, *this);
}

/// matrix operations
template<typename V>
Matrix<V> Matrix<V>::transpose() const {
    Matrix<V> retval(shape.second, shape.first);
    for (Size_T i = 0; i < shape.second; ++i) {
        for (Size_T j = 0; j < shape.first; ++j) {
            retval.unsafe_at(i, j) = this->unsafe_at(j, i);
        }
    }
    return retval;
}

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

/// basic
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

// iterator
template<typename V>
typename Matrix<V>::RawIterator Matrix<V>::raw_begin() {
    return data.begin();
}

template<typename V>
typename Matrix<V>::RawIterator Matrix<V>::raw_end() {
    return data.end();
}

template<typename V>
typename Matrix<V>::ConstRawIterator Matrix<V>::raw_const_begin() const {
    return data.begin();
}

template<typename V>
typename Matrix<V>::ConstRawIterator Matrix<V>::raw_const_end() const {
    return data.end();
}

}
