#pragma once

#include <limits>
#include <iostream>
#include <cstdlib>

#define ASSERT_MSG(expression, message) \
    do { if (!(expression)) { \
        std::cerr << "Assertion failed: (" << #expression << "), \n" \
                  << "\tfunction: " << __FUNCTION__ << '\n' \
                  << "\tfile:     " << __FILE__ << '\n' \
                  << "\tline:     " << __LINE__ << "\n" \
                  << "\tMessage:  " << message << std::endl; \
        std::abort(); \
    } } while (false)
#define ASSERT_EQ(a, b) ASSERT_MSG((a)==(b), "")
#define ASSERT_NE(a, b) ASSERT_MSG((a)!=(b), "")

namespace matlib {

template<typename T>
auto epsilon = T{};

template<>
inline float epsilon<float> = 1e-6;
template<>
inline double epsilon<double> = 1e-6;

template<typename T>
bool MultiplyOverflow(T a, T b) {
    if (a == 0 || b == 0) return false;
    if (a > 0 && b > 0) {
        return b > std::numeric_limits<T>::max() / a;
    } else if (a < 0 && b < 0) {
        return b < std::numeric_limits<T>::max() / a;
    } else {
        return b < std::numeric_limits<T>::min() / a;
    }
}

template<typename T>
inline bool ValueEq(T a, T b) {
    return a == b;
}

template<>
inline bool ValueEq<float>(float a, float b) {
    return (std::abs(a - b) < epsilon<float>);
}

template<>
inline bool ValueEq<double>(double a, double b) {
    return (std::abs(a - b) < epsilon<double>);
}

template<typename V>
struct Default {
    static constexpr V zero = V{};
    static constexpr V one = static_cast<V>(1);
};

}
