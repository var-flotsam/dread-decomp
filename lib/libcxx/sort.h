//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See LICENSE.TXT in this directory for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// std::__sort from libc++ 9.0.1's <algorithm>, adapted as described in README.md.

#pragma once

#include <algorithm>
#include <iterator>

namespace libcxx_adapted {

template <class _Compare, class _RandomAccessIterator>
__attribute__((always_inline)) inline void insertion_sort_3(_RandomAccessIterator __first,
                                                            _RandomAccessIterator __last, _Compare __comp) {
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
    _RandomAccessIterator __j = __first + 2;
    std::__sort3<_Compare>(__first, __first + 1, __j, __comp);
    for (_RandomAccessIterator __i = __j + 1; __i != __last; ++__i) {
        if (__comp(*__i, *__j)) {
            value_type __t(*__i);
            _RandomAccessIterator __k = __j;
            __j = __i;
            do {
                *__j = *__k;
                __j = __k;
            } while (__j != __first && __comp(__t, *--__k));
            *__j = __t;
        }
        __j = __i;
    }
}

template <class _Compare, class _RandomAccessIterator>
void __sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    using std::swap;
    typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
    const difference_type __limit = std::is_trivially_copy_constructible<value_type>::value &&
                                    std::is_trivially_copy_assignable<value_type>::value ? 30 : 6;
    while (true) {
    __restart:
        difference_type __len = __last - __first;
        switch (__len) {
        case 0:
        case 1:
            return;
        case 2:
            if (__comp(*--__last, *__first))
                swap(*__first, *__last);
            return;
        case 3:
            std::__sort3<_Compare>(__first, __first + 1, --__last, __comp);
            return;
        case 4:
            std::__sort4<_Compare>(__first, __first + 1, __first + 2, --__last, __comp);
            return;
        case 5:
            std::__sort5<_Compare>(__first, __first + 1, __first + 2, __first + 3, --__last, __comp);
            return;
        }
        if (__len <= __limit) {
            insertion_sort_3<_Compare>(__first, __last, __comp);
            return;
        }
        // __len > 5
        _RandomAccessIterator __m = __first;
        _RandomAccessIterator __lm1 = __last;
        --__lm1;
        unsigned __n_swaps;
        {
            difference_type __delta;
            if (__len >= 1000) {
                __delta = (unsigned long)__len / 2;
                __m += __delta;
                __delta = (unsigned long)__delta / 2;
                __n_swaps = std::__sort5<_Compare>(__first, __first + __delta, __m, __m + __delta, __lm1, __comp);
            } else {
                __delta = (unsigned long)__len / 2;
                __m += __delta;
                __n_swaps = std::__sort3<_Compare>(__first, __m, __lm1, __comp);
            }
        }
        // *__m is median
        // partition [__first, __m) < *__m and *__m <= [__m, __last)
        _RandomAccessIterator __i = __first;
        _RandomAccessIterator __j = __lm1;
        // j points beyond range to be tested, *__m is known to be <= *__lm1
        // The search going up is known to be guarded but the search coming down isn't.
        // Prime the downward search with a guard.
        if (!__comp(*__i, *__m)) {  // if *__first == *__m
            // *__first == *__m, *__first doesn't go in first part
            // manually guard downward moving __j against __i
            while (true) {
                if (__i == --__j) {
                    // *__first == *__m, *__m <= all other elements
                    // Parition instead into [__first, __i) == *__first and *__first < [__i, __last)
                    ++__i;  // __first + 1
                    __j = __last;
                    if (!__comp(*__first, *--__j)) {  // we need a guard if *__first == *(__last-1)
                        while (true) {
                            if (__i == __j)
                                return;  // [__first, __last) all equivalent elements
                            if (__comp(*__first, *__i)) {
                                swap(*__i, *__j);
                                ++__n_swaps;
                                ++__i;
                                break;
                            }
                            ++__i;
                        }
                    }
                    // [__first, __i) == *__first and *__first < [__j, __last) and __j == __last - 1
                    if (__i == __j)
                        return;
                    while (true) {
                        while (!__comp(*__first, *__i))
                            ++__i;
                        while (__comp(*__first, *--__j))
                            ;
                        if (__i >= __j)
                            break;
                        swap(*__i, *__j);
                        ++__n_swaps;
                        ++__i;
                    }
                    // [__first, __i) == *__first and *__first < [__i, __last)
                    // The first part is sorted, sort the second part
                    __first = __i;
                    goto __restart;
                }
                if (__comp(*__j, *__m)) {
                    swap(*__i, *__j);
                    ++__n_swaps;
                    break;  // found guard for downward moving __j, now use unguarded partition
                }
            }
        }
        // It is known that *__i < *__m
        ++__i;
        // j points beyond range to be tested, *__m is known to be <= *__lm1
        // if not yet partitioned...
        if (__i < __j) {
            // known that *(__i - 1) < *__m
            // known that __i <= __m
            while (true) {
                // __m still guards upward moving __i
                while (__comp(*__i, *__m))
                    ++__i;
                // It is now known that a guard exists for downward moving __j
                while (!__comp(*--__j, *__m))
                    ;
                if (__i > __j)
                    break;
                swap(*__i, *__j);
                ++__n_swaps;
                // It is known that __m != __j
                // If __m just moved, follow it
                if (__m == __i)
                    __m = __j;
                ++__i;
            }
        }
        // [__first, __i) < *__m and *__m <= [__i, __last)
        if (__i != __m && __comp(*__m, *__i)) {
            swap(*__i, *__m);
            ++__n_swaps;
        }
        // [__first, __i) < *__i and *__i <= [__i+1, __last)
        // If we were given a perfect partition, see if insertion sort is quick...
        if (__n_swaps == 0) {
            bool __fs = std::__insertion_sort_incomplete<_Compare>(__first, __i, __comp);
            if (std::__insertion_sort_incomplete<_Compare>(__i + 1, __last, __comp)) {
                if (__fs)
                    return;
                __last = __i;
                continue;
            } else {
                if (__fs) {
                    __first = ++__i;
                    continue;
                }
            }
        }
        // sort smaller range with recursive call and larger with tail recursion elimination
        if (__i - __first < __last - __i) {
            __sort<_Compare>(__first, __i, __comp);
            __first = ++__i;
        } else {
            __sort<_Compare>(__i + 1, __last, __comp);
            __last = __i;
        }
    }
}

}  // namespace libcxx_adapted
