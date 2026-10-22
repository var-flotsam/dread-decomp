// libc++ std::sort helpers instantiated for a list of object pointers, sorted by squared 2D
// distance from a target point (nearest first). The positions come from each object's first
// virtual function. The comparator has internal linkage (an anonymous namespace), so every
// instantiation is internal too: that is what lets std::__sort inline __insertion_sort_3, its only
// caller, as the game's copy does. The helpers are mapped with extern "C" aliases, since a
// declaration using the internal type would be internal and dropped. Names and layouts are
// placeholders.
#include <algorithm>
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec2_7100338620 {
    f32 x;
    f32 y;
    UnkVec2_7100338620(const UnkVec2_7100338620& o) : x(o.x), y(o.y) {}
    f32 DistSq(const UnkVec2_7100338620& p) const { return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y); }
};

struct UnkObj_7100338620 {
    virtual UnkVec2_7100338620 vf_0x0();
};

namespace {
struct UnkNearer_7100338620 {
    const UnkVec2_7100338620* target;

    bool operator()(UnkObj_7100338620* a, UnkObj_7100338620* b) const {
        return target->DistSq(a->vf_0x0()) < target->DistSq(b->vf_0x0());
    }
};
}  // namespace

using P = UnkObj_7100338620*;
using C = UnkNearer_7100338620;
template void std::__sort<C&, P*>(P*, P*, C&);
template unsigned std::__sort3<C&, P*>(P*, P*, P*, C&);
template unsigned std::__sort4<C&, P*>(P*, P*, P*, P*, C&);
template unsigned std::__sort5<C&, P*>(P*, P*, P*, P*, P*, C&);
template bool std::__insertion_sort_incomplete<C&, P*>(P*, P*, C&);

extern "C" {
void fn_7100338620()
    __attribute__((alias("_ZNSt3__16__sortIRN12_GLOBAL__N_120UnkNearer_7100338620EPP17UnkObj_7100338620EEvT0_S7_T_")));
void fn_7100338ed0()
    __attribute__((alias("_ZNSt3__17__sort3IRN12_GLOBAL__N_120UnkNearer_7100338620EPP17UnkObj_7100338620EEjT0_S7_S7_T_")));
void fn_7100339160()
    __attribute__((alias("_ZNSt3__17__sort4IRN12_GLOBAL__N_120UnkNearer_7100338620EPP17UnkObj_7100338620EEjT0_S7_S7_S7_T_")));
void fn_7100339370()
    __attribute__((alias("_ZNSt3__17__sort5IRN12_GLOBAL__N_120UnkNearer_7100338620EPP17UnkObj_7100338620EEjT0_S7_S7_S7_S7_T_")));
// NON_MATCHING(minor): the store of the moved value at the end of the shift folds its address
// into a pre-indexed str where the original keeps a separate add (blocks are laid out
// differently). Same behaviour.
void fn_7100339610()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_120UnkNearer_7100338620EPP17UnkObj_7100338620EEbT0_S7_T_")));
}
