#pragma once

// matlib
#include "common.hpp"


namespace matlib {

/// Range
struct Range {
    Size_T a{}, b{};
    Range(Size_T a_ = 0, Size_T b_ = 0);
    bool empty() const;
    bool include(const Range& rhs);
};

}