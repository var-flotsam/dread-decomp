// libc++ std::sort helpers instantiated for 0x30-byte entries, nearest first: by the squared
// distance from the comparator's point to the entry's position with z taken as 0, compared
// through the sign of the difference (as in fn_7100354e10.cpp). The comparator type has external
// linkage, so std::__sort keeps __insertion_sort_3 out of line as the game's copy does. Names and
// layouts are placeholders.
//
// The position's x and z are union members: their access type is then char, so a copy reads them
// again after each store as in the game.
//
// The two insertion sorts are not exact: when the held entry is stored back, the original
// addresses its middle words from the loop's base register, ours from the destination pointer.
// Same behaviour.
#include <algorithm>
#include "types.h"

struct UnkPos_71002aa690 {
    union {
        f32 x;
    };
    f32 y;
    union {
        f32 z;
    };
};

struct UnkVec3_71002aa690 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkEntry_71002aa690 {
    u32 field_0x0;
    UnkPos_71002aa690 pos;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;

#define UNK_COPY_71002aa690(o)                                                                     \
    field_0x0 = o.field_0x0;                                                                       \
    pos.x = o.pos.x;                                                                               \
    pos.y = o.pos.y;                                                                               \
    pos.z = o.pos.z;                                                                               \
    field_0x10 = o.field_0x10;                                                                     \
    field_0x14 = o.field_0x14;                                                                     \
    field_0x18 = o.field_0x18;                                                                     \
    field_0x1c = o.field_0x1c;                                                                     \
    field_0x20 = o.field_0x20;                                                                     \
    field_0x24 = o.field_0x24;                                                                     \
    field_0x28 = o.field_0x28;                                                                     \
    field_0x2c = o.field_0x2c;

    UnkEntry_71002aa690(const UnkEntry_71002aa690& o) { UNK_COPY_71002aa690(o) }
    UnkEntry_71002aa690& operator=(const UnkEntry_71002aa690& o) {
        UNK_COPY_71002aa690(o)
        return *this;
    }
#undef UNK_COPY_71002aa690
};

static inline f32 Sign_71002aa690(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

struct UnkCmp_71002aa690 {
    UnkVec3_71002aa690 pos;

    f32 DistSq(const UnkVec3_71002aa690& p) const {
        return (pos.x - p.x) * (pos.x - p.x) + (pos.y - p.y) * (pos.y - p.y) +
               (pos.z - p.z) * (pos.z - p.z);
    }
    bool operator()(const UnkEntry_71002aa690& a, const UnkEntry_71002aa690& b) const {
        UnkVec3_71002aa690 pb = {b.pos.x, b.pos.y, 0.0f};
        f32 db = DistSq(pb);
        UnkVec3_71002aa690 pa = {a.pos.x, a.pos.y, 0.0f};
        f32 da = DistSq(pa);
        return (s32)Sign_71002aa690(db - da) > 0;
    }
};

using E = UnkEntry_71002aa690;
using C = UnkCmp_71002aa690;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);

// extern "C", so the names stay plain.
extern "C" {
void fn_71002aa690()
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEvT0_S5_T_")));
void fn_71002ab060()
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEjT0_S5_S5_T_")));
void fn_71002ab4a0()
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEjT0_S5_S5_S5_T_")));
void fn_71002ab7f0()
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEjT0_S5_S5_S5_S5_T_")));
// NON_MATCHING(minor): the held entry's store-back addressing, see the note at the top of the file.
void fn_71002abc50()
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEvT0_S5_T_")));
// NON_MATCHING(minor): the held entry's store-back addressing, see the note at the top of the file.
void fn_71002abe10()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_71002aa690P19UnkEntry_71002aa690EEbT0_S5_T_")));
}
