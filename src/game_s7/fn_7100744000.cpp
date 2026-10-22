// libc++ std::sort helpers instantiated for a list of 8-byte entries (through its own iterator
// class), ordered by a float key and then by id. The comparator has internal linkage, so
// std::__sort inlines __insertion_sort_3 as the game's copy does. Names and layouts are
// placeholders.
//
// The key is a union member: its access type is then char, which may alias the id, so the key is
// read again after a store as in the game. The swap assigns the two members in turn.
//
// std::__sort is one instruction off: in its partition loop the original advances the pointer and
// then loads the key, where ours loads with a pre-indexed address. Same behaviour.
#include <algorithm>
#include "UnkIter.h"
#include "types.h"

struct UnkEntry_7100744000 {
    union {
        f32 key;
    };
    u32 id;

    UnkEntry_7100744000(const UnkEntry_7100744000& o) : key(o.key), id(o.id) {}
    UnkEntry_7100744000& operator=(const UnkEntry_7100744000& o) {
        key = o.key;
        id = o.id;
        return *this;
    }
};

__attribute__((always_inline)) inline void swap(UnkEntry_7100744000& a, UnkEntry_7100744000& b) {
    f32 key = a.key;
    a.key = b.key;
    b.key = key;
    u32 id = a.id;
    a.id = b.id;
    b.id = id;
}

namespace {

struct UnkCmp_7100744000 {
    bool operator()(const UnkEntry_7100744000& a, const UnkEntry_7100744000& b) const {
        if (a.key < b.key)
            return true;
        if (b.key < a.key)
            return false;
        return a.id < b.id;
    }
};

}  // namespace

using E = UnkEntry_7100744000;
using I = UnkIter<E>;
using C = UnkCmp_7100744000;
template void std::__sort<C&, I>(I, I, C&);
template unsigned std::__sort3<C&, I>(I, I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template unsigned std::__sort5<C&, I>(I, I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

// extern "C", so the names stay plain.
extern "C" {
// NON_MATCHING(minor): one load in the partition loop, see the note at the top of the file.
void fn_7100744000()
    __attribute__((alias("_ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7100744000E7UnkIterI19UnkEntry_7100744000EEEvT0_S7_T_")));
void fn_71007445c0()
    __attribute__((alias("_ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_7100744000E7UnkIterI19UnkEntry_7100744000EEEjT0_S7_S7_T_")));
void fn_7100744740()
    __attribute__((alias("_ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7100744000E7UnkIterI19UnkEntry_7100744000EEEjT0_S7_S7_S7_T_")));
void fn_7100744860()
    __attribute__((alias("_ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_7100744000E7UnkIterI19UnkEntry_7100744000EEEjT0_S7_S7_S7_S7_T_")));
void fn_71007449e0()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7100744000E7UnkIterI19UnkEntry_7100744000EEEbT0_S7_T_")));
}
