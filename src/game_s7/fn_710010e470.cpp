// libc++ std::sort helpers instantiated for 0x14-byte boxes (a 2D rectangle and a word), sorted
// by the centre's coordinate on the comparator's axis. std::__sort is the adapted copy in
// lib/libcxx/sort.h. The tree builder that sorts with them (fn_710010d9fc) is at the end. Names and
// layouts are placeholders.
//
// None of the five match yet. The original reloads a box's first two words for the swaps (as
// integers) instead of reusing the values loaded for its centre, and computes each centre's y
// before its x; ours keeps the loaded floats in registers, which shifts every register after
// the first comparison. Writing the box as two vectors fixes the y-first order but copies them
// as doublewords, and no layout tried reproduces the reloads. Same behaviour.
#include <algorithm>
#include <new>
#include "../../lib/libcxx/sort.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkVec2_710010e470 {
    f32 v[2];
    UnkVec2_710010e470(f32 x, f32 y) {
        v[0] = x;
        v[1] = y;
    }
    f32 operator[](u32 i) const { return v[i]; }
};

// Copied member by member (not trivially copyable).
struct UnkBox_710010e470 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    u32 field_0x10;

    UnkBox_710010e470(const UnkBox_710010e470& o)
        : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y), field_0x10(o.field_0x10) {}
    UnkBox_710010e470& operator=(const UnkBox_710010e470& o) {
        min_x = o.min_x;
        min_y = o.min_y;
        max_x = o.max_x;
        max_y = o.max_y;
        field_0x10 = o.field_0x10;
        return *this;
    }
    UnkVec2_710010e470 Center() const {
        return UnkVec2_710010e470((min_x + max_x) * 0.5f, (min_y + max_y) * 0.5f);
    }
};

struct UnkByAxis_710010e470 {
    u32 axis;
    bool operator()(const UnkBox_710010e470& a, const UnkBox_710010e470& b) const {
        return a.Center()[axis] < b.Center()[axis];
    }
};

using E = UnkBox_710010e470;
using C = UnkByAxis_710010e470;
template void libcxx_adapted::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);

// extern "C", so the names stay plain.
extern "C" {
// NON_MATCHING(minor): register allocation and reloads, see the note at the top of the file.
void fn_710010e470()
    __attribute__((alias("_ZN14libcxx_adapted6__sortIR20UnkByAxis_710010e470P17UnkBox_710010e470EEvT0_S5_T_")));
// NON_MATCHING(minor): register allocation and reloads, see the note at the top of the file.
void fn_710010ed20()
    __attribute__((alias("_ZNSt3__17__sort3IR20UnkByAxis_710010e470P17UnkBox_710010e470EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): register allocation and reloads, see the note at the top of the file.
void fn_710010efe0()
    __attribute__((alias("_ZNSt3__17__sort4IR20UnkByAxis_710010e470P17UnkBox_710010e470EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): register allocation and reloads, see the note at the top of the file.
void fn_710010f200()
    __attribute__((alias("_ZNSt3__17__sort5IR20UnkByAxis_710010e470P17UnkBox_710010e470EEjT0_S5_S5_S5_S5_T_")));
// NON_MATCHING(minor): register allocation and reloads, see the note at the top of the file.
void fn_710010f4b0()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR20UnkByAxis_710010e470P17UnkBox_710010e470EEbT0_S5_T_")));
}

// ---- fn_710010d9fc: building the box tree ----

// The boxes and the tree's nodes (base/UnkVector.h's layout); fn_710010e364 resizes either.
struct UnkBoxArray_710010e470 {
    E* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};
void fn_710010e364(UnkBoxArray_710010e470* self, u32 n);

// The empty box built from its two corners, the box around a range, and the range's split
// position and spread (both returned in memory).
void fn_710010d61c(E* out, const f32* corners);
void fn_710010d68c(E* box, E* const* first, E* const* last);
struct UnkBoxSplit_710010e470 {
    f32 v[2];
    f32 field_0x8[3];
    UnkBoxSplit_710010e470(const UnkBoxSplit_710010e470& other);
};
UnkBoxSplit_710010e470 fn_710010df80(void*, const UnkBoxArray_710010e470* boxes, u32 first, u32 last);
UnkBoxSplit_710010e470 fn_710010e15c(void*, const UnkBoxArray_710010e470* boxes, u32 first, u32 last,
                                     const UnkBoxSplit_710010e470* split);

// Builds the tree over boxes [first, last): a node with the range's bounds, then either the box itself
// (a leaf) or the two halves split on the axis with the larger spread, at the split position (or at the
// middle after a sort when that leaves one side with less than a third). An inner node's word is the
// index after its subtree.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_710010d9fc(UnkBoxArray_710010e470* tree, UnkBoxArray_710010e470* boxes, u32 first, u32 last) {
    fn_710010e364(tree, tree->count + 1);
    E* node = tree->count ? &tree->data[tree->count - 1] : tree->data;
    {
        const f32 empty[4] = {3.4028235e38f, 3.4028235e38f, -3.4028235e38f, -3.4028235e38f};
        alignas(4) u8 raw[sizeof(E)];
        E* fresh = reinterpret_cast<E*>(raw);
        fn_710010d61c(fresh, empty);
        *node = *fresh;
    }
    {
        E* begin = boxes->data + first;
        E* end = boxes->data + last;
        fn_710010d68c(node, &begin, &end);
    }
    u32 n = last - first;
    u32 word;
    if (n < 2) {
        if (boxes->growable && boxes->count <= first) {
            if (boxes->capacity <= first && boxes->step != 0) {
                u32 c = (first / boxes->step + 1) * boxes->step;
                if (boxes->capacity < c) {
                    E* p = static_cast<E*>(boxes->allocator->vf_0x40((u64)c * sizeof(E), 8, boxes->flags));
                    if (boxes->data) {
                        for (u32 i = 0; i < boxes->count; ++i)
                            new (&p[i]) E(boxes->data[i]);
                        boxes->allocator->vf_0x48(boxes->data);
                    }
                    boxes->data = p;
                    boxes->capacity = c;
                }
            }
            fn_710010e364(boxes, first + 1);
        }
        const E& box = boxes->data[first];
        node->min_x = box.min_x;
        node->min_y = box.min_y;
        node->max_x = box.max_x;
        node->max_y = box.max_y;
        word = box.field_0x10;
    } else {
        UnkBoxSplit_710010e470 split = fn_710010df80(nullptr, boxes, first, last);
        UnkBoxSplit_710010e470 spread = fn_710010e15c(nullptr, boxes, first, last, &split);
        u32 axis = spread.v[0] < spread.v[1];
        f32 at = split.v[axis];
        E* lo = boxes->data + first;
        E* hi = boxes->data + last;
        E* mid = std::partition(lo, hi, [axis, at](const E& b) { return b.Center()[axis] < at; });
        u32 m = (u32)(mid - boxes->data);
        if (m <= n / 3 + first || ~(n / 3) + last <= m) {
            C by_axis{axis};
            libcxx_adapted::__sort<C&, E*>(boxes->data + first, boxes->data + last, by_axis);
            m = first + (n >> 1);
        }
        fn_710010d9fc(tree, boxes, first, m);
        fn_710010d9fc(tree, boxes, m, last);
        word = tree->count;
    }
    node->field_0x10 = word;
}
