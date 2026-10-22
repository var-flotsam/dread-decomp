// libc++ std::sort helpers instantiated for a list of object pointers (through its own iterator
// class) with a plain function-pointer comparator. With an external comparator type std::__sort
// keeps __insertion_sort_3 out of line, as the game's copy does; the game's __sort3 and __sort5
// were folded into other copies. Names are placeholders.
#include <algorithm>
#include "UnkIter.h"
#include "types.h"

struct UnkObj_7100742750;

using P = UnkObj_7100742750*;
using I = UnkIter<P>;
using C = bool (*)(P, P);
template void std::__sort<C&, I>(I, I, C&);
template void std::__insertion_sort_3<C&, I>(I, I, C&);
template unsigned std::__sort3<C&, I>(I, I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template unsigned std::__sort5<C&, I>(I, I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

// extern "C", so the names stay plain.
extern "C" {
void fn_7100742750()
    __attribute__((alias("_ZNSt3__16__sortIRPFbP17UnkObj_7100742750S2_E7UnkIterIS2_EEEvT0_S8_T_")));
void fn_7100742d60()
    __attribute__((alias("_ZNSt3__17__sort4IRPFbP17UnkObj_7100742750S2_E7UnkIterIS2_EEEjT0_S8_S8_S8_T_")));
void fn_7100742ee0()
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IRPFbP17UnkObj_7100742750S2_E7UnkIterIS2_EEEvT0_S8_T_")));
void fn_7100743030()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRPFbP17UnkObj_7100742750S2_E7UnkIterIS2_EEEbT0_S8_T_")));
}
