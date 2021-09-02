#pragma once

#include <limits>

template<typename T>
struct Bound final {
    T lowerBound, upperBound;
    constexpr Bound()
        : lowerBound(std::numeric_limits<T>::max()),
          upperBound(std::numeric_limits<T>::min()) {}
    inline Bound &merge(T x) {
        if (x < lowerBound) lowerBound = x;
        if (x > upperBound) upperBound = x;
        return *this;
    }
    inline Bound &merge(Bound other) {
        if (other.lowerBound < lowerBound) lowerBound = other.lowerBound;
        if (other.upperBound > upperBound) upperBound = other.upperBound;
        return *this;
    }
};