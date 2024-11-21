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

}
