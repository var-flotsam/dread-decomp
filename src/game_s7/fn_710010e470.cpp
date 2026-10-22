// libc++ std::sort helpers instantiated for 0x14-byte boxes (a 2D rectangle and a word), sorted
// by the centre's coordinate on the comparator's axis. std::__sort is the adapted copy in
// lib/libcxx/sort.h. Names and layouts are placeholders.
//
// None of the five match yet. The original reloads a box's first two words for the swaps (as
// integers) instead of reusing the values loaded for its centre, and computes each centre's y
// before its x; ours keeps the loaded floats in registers, which shifts every register after
// the first comparison. Writing the box as two vectors fixes the y-first order but copies them
// as doublewords, and no layout tried reproduces the reloads. Same behaviour.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
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
