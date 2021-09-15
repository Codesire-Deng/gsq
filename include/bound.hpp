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
    inline Bound &slack(T x) {
        lowerBound -= x;
        upperBound += x;
        return *this;
    }
    inline T length() const { return upperBound - lowerBound; }
};

template<typename T, size_t N>
struct Bounds final {
    Bound<T> bounds[N];
    constexpr Bounds() {}
    inline Bounds &merge(const Bounds &other) {
        for (int i = 0; i < N; ++i) { bounds[i].merge(other.bounds[i]); }
        return *this;
    }
    inline Bounds &slack(T x) {
        for (int i = 0; i < N; ++i) { bounds[i].slack(x); }
        return *this;
    }
};