// libc++ std::sort helpers instantiated for a list of 8-byte entries (through its own iterator
// class), ordered by key in two runs: keys above the reference entry's key come first, each run
// ascending. The comparator has internal linkage, so std::__sort inlines __insertion_sort_3 as
// the game's copy does. Names and layouts are placeholders.
#include <algorithm>
#include "UnkIter.h"
#include "types.h"

struct UnkEntry_7100743340 {
    u32 field_0x0;
    u32 key;
};

namespace {

struct UnkCmp_7100743340 {
    const UnkEntry_7100743340* ref;

    bool operator()(const UnkEntry_7100743340& a, const UnkEntry_7100743340& b) const {
        if (a.key <= ref->key) {
            if (b.key > ref->key)
                return false;
            return a.key < b.key;
        }
        if (b.key > ref->key)
            return a.key < b.key;
        return true;
    }
};

}  // namespace

using E = UnkEntry_7100743340;
using I = UnkIter<E>;
using C = UnkCmp_7100743340;
template void std::__sort<C&, I>(I, I, C&);
template unsigned std::__sort3<C&, I>(I, I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template unsigned std::__sort5<C&, I>(I, I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

// extern "C", so the names stay plain.
extern "C" {
void fn_7100743340()
    __attribute__((alias("_ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7100743340E7UnkIterI19UnkEntry_7100743340EEEvT0_S7_T_")));
void fn_71007439c0()
    __attribute__((alias("_ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_7100743340E7UnkIterI19UnkEntry_7100743340EEEjT0_S7_S7_T_")));
void fn_7100743b30()
    __attribute__((alias("_ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7100743340E7UnkIterI19UnkEntry_7100743340EEEjT0_S7_S7_S7_T_")));
void fn_7100743c60()
    __attribute__((alias("_ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_7100743340E7UnkIterI19UnkEntry_7100743340EEEjT0_S7_S7_S7_S7_T_")));
void fn_7100743dd0()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7100743340E7UnkIterI19UnkEntry_7100743340EEEbT0_S7_T_")));
}
