// libc++ std::sort helpers instantiated for 0x28-byte entries: by the key at +0xc, and on equal
// keys nearest first by the squared 2D distance from the comparator's point. The comparator type
// has external linkage, so std::__sort keeps __insertion_sort_3 out of line as the game's copy
// does. Names and layouts are placeholders.
//
// The coordinates are union members: their access type is then char, so a copy reads them again
// after each store as in the game.
//
// __sort3 and the two insertion sorts are not exact. In __sort3 the original loads the first
// entry once, before the branch that picks which pair to swap; ours loads it in each branch. The
// original's insertion sorts are a few instructions longer: they hold the moved entry's fields and
// the store addresses in other registers. Same behaviour.
#include <algorithm>
#include "types.h"

struct UnkSub_7100c75e40 {
    u32 field_0x0;
    f32 key;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

struct UnkEntry_7100c75e40 {
    union {
        f32 x;
    };
    union {
        f32 y;
    };
    UnkSub_7100c75e40 sub;
    u32 field_0x1c;
    u32 field_0x20;
    u16 field_0x24;

    UnkEntry_7100c75e40(const UnkEntry_7100c75e40& o) = default;
    UnkEntry_7100c75e40& operator=(const UnkEntry_7100c75e40& o) {
        x = o.x;
        y = o.y;
        sub = o.sub;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        return *this;
    }
};

struct UnkCmp_7100c75e40 {
    f32 x;
    f32 y;

    f32 DistSq(const UnkEntry_7100c75e40& e) const {
        return (e.x - x) * (e.x - x) + (e.y - y) * (e.y - y);
    }
    bool operator()(const UnkEntry_7100c75e40& a, const UnkEntry_7100c75e40& b) const {
        if (a.sub.key == b.sub.key)
            return DistSq(a) < DistSq(b);
        return a.sub.key < b.sub.key;
    }
};

using E = UnkEntry_7100c75e40;
using C = UnkCmp_7100c75e40;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);

// extern "C", so the names stay plain.
extern "C" {
void fn_7100c75e40()
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEvT0_S5_T_")));
// NON_MATCHING(minor): the first entry is loaded in each branch, see the note at the top of the file.
void fn_7100c76750()
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEjT0_S5_S5_T_")));
void fn_7100c76b20()
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEjT0_S5_S5_S5_T_")));
void fn_7100c76e10()
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEjT0_S5_S5_S5_S5_T_")));
// NON_MATCHING(minor): registers and store addressing, see the note at the top of the file.
void fn_7100c771e0()
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEvT0_S5_T_")));
// NON_MATCHING(minor): registers and store addressing, see the note at the top of the file.
void fn_7100c77460()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_7100c75e40P19UnkEntry_7100c75e40EEbT0_S5_T_")));
}
