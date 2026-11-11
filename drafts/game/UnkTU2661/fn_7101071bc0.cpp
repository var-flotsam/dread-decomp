// CCutscene unit: libc++'s __stable_sort for an array of node pointers, ordered by fn_710019536c's
// value, largest first. Sorts in place, using the scratch buffer when it is large enough.
// Names and layouts are placeholders.
#include <algorithm>
#include "types.h"

struct UnkObj_7101072a80;
u32 fn_710019536c(UnkObj_7101072a80* obj);

struct UnkOrder_7101071bc0 {
    bool operator()(UnkObj_7101072a80* a, UnkObj_7101072a80* b) const {
        return fn_710019536c(a) > fn_710019536c(b);
    }
};

using Node = UnkObj_7101072a80*;
using Order = UnkOrder_7101071bc0;

namespace std {
inline namespace __1 {
// Out-of-line pieces of the same sort.
template <>
void __stable_sort_move<Order&, Node*>(Node* first, Node* last, Order& order, ptrdiff_t len,
                                       Node* out) asm("fn_7101071ee0");
template <>
void __inplace_merge<Order&, Node*>(Node* first, Node* middle, Node* last, Order& order, ptrdiff_t len1,
                                    ptrdiff_t len2, Node* buf, ptrdiff_t buf_size) asm("fn_71010721f0");
}  // namespace __1
}  // namespace std

void fn_7101071bc0(Node* first, Node* last, Order& order, ptrdiff_t len, Node* buf, ptrdiff_t buf_size) {
    switch (len) {
    case 0:
    case 1:
        return;
    case 2:
        if (order(*--last, *first))
            std::swap(*first, *last);
        return;
    }
    if (len <= 128) {
        // Short ranges: insertion sort.
        if (first == last)
            return;
        for (Node* cur = first + 1; cur != last; ++cur) {
            Node value = *cur;
            Node* slot = cur;
            for (Node* prev = cur; prev != first; --slot) {
                --prev;
                if (!order(value, *prev))
                    break;
                *slot = *prev;
            }
            *slot = value;
        }
        return;
    }
    ptrdiff_t half = len / 2;
    Node* middle = first + half;
    if (len <= buf_size) {
        // Sort each half into the buffer, then merge them back.
        std::__stable_sort_move<Order&>(first, middle, order, half, buf);
        std::__stable_sort_move<Order&>(middle, last, order, len - half, buf + half);
        std::__merge_move_assign<Order&>(buf, buf + half, buf + half, buf + len, first, order);
        return;
    }
    fn_7101071bc0(first, middle, order, half, buf, buf_size);
    fn_7101071bc0(middle, last, order, len - half, buf, buf_size);
    std::__inplace_merge<Order&>(first, middle, last, order, half, len - half, buf, buf_size);
}
