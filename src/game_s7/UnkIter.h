// A random-access iterator over a plain array, as the game's containers have. Sorting through it
// instead of through raw pointers changes how libc++'s std::sort helpers are inlined and how their
// insertion loops walk the elements, which several sort families need to match.
#pragma once

#include <iterator>

#include "types.h"

template <class T>
struct UnkIter {
    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef s64 difference_type;
    typedef T* pointer;
    typedef T& reference;
    T* p;

    reference operator*() const { return *p; }
    UnkIter& operator++() {
        ++p;
        return *this;
    }
    UnkIter& operator--() {
        --p;
        return *this;
    }
    UnkIter operator++(int) {
        UnkIter t = *this;
        ++p;
        return t;
    }
    UnkIter operator--(int) {
        UnkIter t = *this;
        --p;
        return t;
    }
    UnkIter& operator+=(s64 n) {
        p += n;
        return *this;
    }
    UnkIter operator+(s64 n) const {
        UnkIter t = *this;
        t.p += n;
        return t;
    }
    UnkIter operator-(s64 n) const {
        UnkIter t = *this;
        t.p -= n;
        return t;
    }
    s64 operator-(const UnkIter& o) const { return p - o.p; }
    bool operator==(const UnkIter& o) const { return p == o.p; }
    bool operator!=(const UnkIter& o) const { return p != o.p; }
    bool operator<(const UnkIter& o) const { return p < o.p; }
    bool operator>(const UnkIter& o) const { return p > o.p; }
    bool operator>=(const UnkIter& o) const { return p >= o.p; }
    bool operator<=(const UnkIter& o) const { return p <= o.p; }
};
