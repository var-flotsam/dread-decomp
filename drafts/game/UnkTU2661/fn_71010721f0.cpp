// Unit 2661 (CCutscene::CTunableCutscene): the in-place merge step of the stable sort of a node's
// children, deepest subtree first (libc++'s __inplace_merge, with its helpers written out).
// Names and layouts are placeholders.
#include <iterator>

#include "types.h"

struct UnkObj_7101072a80;
u32 ComputeSubtreeDepth(UnkObj_7101072a80* obj);

typedef UnkObj_7101072a80 UnkDepthNode;

// Deeper subtrees sort first.
struct UnkDepthOrder {
    bool operator()(UnkDepthNode* a, UnkDepthNode* b) const { return ComputeSubtreeDepth(a) > ComputeSubtreeDepth(b); }
};

// The first element of the sorted range [first, last) that `value` sorts before.
static inline UnkDepthNode** UnkUpperBound(UnkDepthNode** first, UnkDepthNode** last, UnkDepthNode* const& value,
                                           const UnkDepthOrder& order) {
    s64 len = last - first;
    while (len != 0) {
        s64 half = (u64)len >> 1;
        UnkDepthNode** mid = first + half;
        if (order(value, *mid)) {
            len = half;
        } else {
            first = mid + 1;
            len -= half + 1;
        }
    }
    return first;
}

// The first element of the sorted range [first, last) that does not sort before `value`.
static inline UnkDepthNode** UnkLowerBound(UnkDepthNode** first, UnkDepthNode** last, UnkDepthNode* const& value,
                                           const UnkDepthOrder& order) {
    s64 len = last - first;
    while (len != 0) {
        s64 half = (u64)len >> 1;
        UnkDepthNode** mid = first + half;
        if (order(*mid, value)) {
            first = mid + 1;
            len -= half + 1;
        } else {
            len = half;
        }
    }
    return first;
}

static inline s64 UnkGcd(s64 x, s64 y) {
    do {
        s64 t = x % y;
        x = y;
        y = t;
    } while (y);
    return x;
}

// Copies [first, last) to `out` onwards; returns the end of the copy.
template <class In, class Out>
static inline Out UnkMove(In first, In last, Out out) {
    for (; first != last; ++first, ++out)
        *out = *first;
    return out;
}

// Copies [first, last) to end at `out`; returns the start of the copy.
static inline UnkDepthNode** UnkMoveBackward(UnkDepthNode** first, UnkDepthNode** last, UnkDepthNode** out) {
    while (first != last)
        *--out = *--last;
    return out;
}

static inline void UnkSwapRanges(UnkDepthNode** a, UnkDepthNode** a_end, UnkDepthNode** b) {
    for (; a != a_end; ++a, ++b) {
        UnkDepthNode* t = *a;
        *a = *b;
        *b = t;
    }
}

// Moves the first element of [first, last) to the back.
static inline UnkDepthNode** UnkRotateLeft(UnkDepthNode** first, UnkDepthNode** last) {
    UnkDepthNode* tmp = *first;
    UnkDepthNode** back = UnkMove(first + 1, last, first);
    *back = tmp;
    return back;
}

// Moves the last element of [first, last) to the front.
static inline UnkDepthNode** UnkRotateRight(UnkDepthNode** first, UnkDepthNode** last) {
    UnkDepthNode** back = last - 1;
    UnkDepthNode* tmp = *back;
    UnkDepthNode** second = UnkMoveBackward(first, back, last);
    *first = tmp;
    return second;
}

// Rotates through cycles of elements, as many as the lengths' greatest common divisor.
static inline UnkDepthNode** UnkRotateCycles(UnkDepthNode** first, UnkDepthNode** middle, UnkDepthNode** last) {
    s64 len1 = middle - first;
    s64 len2 = last - middle;
    if (len1 == len2) {
        UnkSwapRanges(first, middle, middle);
        return middle;
    }
    s64 cycles = UnkGcd(len1, len2);
    for (UnkDepthNode** p = first + cycles; p != first;) {
        UnkDepthNode* tmp = *--p;
        UnkDepthNode** hole = p;
        UnkDepthNode** next = hole + len1;
        do {
            *hole = *next;
            hole = next;
            s64 left = last - next;
            if (len1 < left)
                next += len1;
            else
                next = first + (len1 - left);
        } while (next != p);
        *hole = tmp;
    }
    return first + len2;
}

// Rotates [first, last) so that `middle` comes first; returns where `first`'s element went.
static inline UnkDepthNode** UnkRotate(UnkDepthNode** first, UnkDepthNode** middle, UnkDepthNode** last) {
    if (first == middle)
        return last;
    if (middle == last)
        return first;
    if (first + 1 == middle)
        return UnkRotateLeft(first, last);
    if (middle + 1 == last)
        return UnkRotateRight(first, last);
    return UnkRotateCycles(first, middle, last);
}

// Merges the sorted ranges [first1, last1) and [first2, last2) into `out`, where [first2, last2)
// already ends the output.
template <class In1, class In2, class Out, class Order>
static inline void UnkHalfMerge(In1 first1, In1 last1, In2 first2, In2 last2, Out out, Order& order) {
    for (; first1 != last1; ++out) {
        if (first2 == last2) {
            UnkMove(first1, last1, out);
            return;
        }
        if (order(*first2, *first1)) {
            *out = *first2;
            ++first2;
        } else {
            *out = *first1;
            ++first1;
        }
    }
}

// An order's reverse, for merging from the back.
template <class Order>
struct UnkReversedOrder {
    Order& order;
    template <class T>
    bool operator()(const T& a, const T& b) const {
        return order(b, a);
    }
};

// Merges the sorted ranges [first, middle) and [middle, last), the shorter one moved to `buf`.
static inline void UnkBufferedMerge(UnkDepthNode** first, UnkDepthNode** middle, UnkDepthNode** last,
                                    UnkDepthOrder& order, s64 len1, s64 len2, UnkDepthNode** buf) {
    if (len1 <= len2) {
        UnkDepthNode** buf_end = buf;
        for (UnkDepthNode** in = first; in != middle; ++in, ++buf_end)
            *buf_end = *in;
        UnkHalfMerge(buf, buf_end, middle, last, first, order);
    } else {
        UnkDepthNode** buf_end = buf;
        for (UnkDepthNode** in = middle; in != last; ++in, ++buf_end)
            *buf_end = *in;
        // From the back: the later of the two last elements goes last.
        typedef std::reverse_iterator<UnkDepthNode**> Back;
        UnkReversedOrder<UnkDepthOrder> reversed = {order};
        UnkHalfMerge(Back(buf_end), Back(buf), Back(middle), Back(first), Back(last), reversed);
    }
}

// Skips the elements at the front of the first range that stay where they are; false when that
// leaves none.
static inline bool UnkSkipPlaced(UnkDepthNode**& first, UnkDepthNode** middle, s64& len1, UnkDepthOrder& order) {
    for (;; ++first, --len1) {
        if (len1 == 0)
            return false;
        if (order(*middle, *first))
            return true;
    }
}

// Merges the sorted ranges [first, middle) and [middle, last) of `len1` and `len2` elements, with
// a buffer of `buf_size` elements.
void fn_71010721f0(UnkDepthNode** first, UnkDepthNode** middle, UnkDepthNode** last, UnkDepthOrder& order,
                   s64 len1, s64 len2, UnkDepthNode** buf, s64 buf_size) {
    while (true) {
        if (len2 == 0)
            return;
        if (len2 <= buf_size || len1 <= buf_size) {
            UnkBufferedMerge(first, middle, last, order, len1, len2, buf);
            return;
        }
        if (!UnkSkipPlaced(first, middle, len1, order))
            return;
        UnkDepthNode** cut1;
        UnkDepthNode** cut2;
        s64 len11;
        s64 len21;
        if (len1 < len2) {
            len21 = len2 / 2;
            cut2 = middle + len21;
            cut1 = UnkUpperBound(first, middle, *cut2, order);
            len11 = cut1 - first;
        } else {
            if (len1 == 1) {
                UnkDepthNode* t = *first;
                *first = *middle;
                *middle = t;
                return;
            }
            len11 = len1 / 2;
            cut1 = first + len11;
            cut2 = UnkLowerBound(middle, last, *cut1, order);
            len21 = cut2 - middle;
        }
        s64 len12 = len1 - len11;
        s64 len22 = len2 - len21;
        middle = UnkRotate(cut1, middle, cut2);
        // Recurse on the smaller half, loop on the larger.
        if (len11 + len21 < len12 + len22) {
            fn_71010721f0(first, cut1, middle, order, len11, len21, buf, buf_size);
            first = middle;
            middle = cut2;
            len1 = len12;
            len2 = len22;
        } else {
            fn_71010721f0(middle, cut2, last, order, len12, len22, buf, buf_size);
            last = middle;
            middle = cut1;
            len1 = len11;
            len2 = len21;
        }
    }
}
