// libc++ std::sort helpers instantiated for a list of object pointers (through its own iterator
// class), ordered by the s32 at +0x18, largest first. The comparator has internal linkage, so
// std::__sort inlines __insertion_sort_3 as the game's copy does; __sort3 and __sort5 are inlined
// too. The field is a union member, so it is read again after an element store as in the game.
// Names and layouts are placeholders.
#include <algorithm>
#include "../../game_s7/UnkIter.h"
#include "types.h"

struct UnkObj_7100256810 {
    u8 pad_0x0[0x18];
    union {
        s32 field_0x18;
    };
};

namespace {

struct UnkCmp_7100256810 {
    bool operator()(const UnkObj_7100256810* a, const UnkObj_7100256810* b) const {
        return a->field_0x18 > b->field_0x18;
    }
};

}  // namespace

using I = UnkIter<UnkObj_7100256810*>;
using C = UnkCmp_7100256810;
template void std::__sort<C&, I>(I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

// extern "C", so the names stay plain.
extern "C" {
void fn_7100256810()
    __attribute__((alias("_ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7100256810E7UnkIterIP17UnkObj_7100256810EEEvT0_S8_T_")));
void fn_7100256e90()
    __attribute__((alias("_ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7100256810E7UnkIterIP17UnkObj_7100256810EEEjT0_S8_S8_S8_T_")));
void fn_7100256ff0()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7100256810E7UnkIterIP17UnkObj_7100256810EEEbT0_S8_T_")));
}
