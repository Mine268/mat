// std
#include <algorithm>
// matlib
#include "range.hpp"


namespace matlib {

Range::Range(Size_T a_, Size_T b_) {
    a = std::min(a_, b_);
    b = std::max(a_, b_);
}

bool Range::empty() const {
    return a <= b;
}

bool Range::include(const Range& rhs) {
    return a <= rhs.a && b >= rhs.b;
}

}
