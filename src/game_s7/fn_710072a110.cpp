// libc++ std::sort helpers instantiated for a container of object pointers (through its own
// iterator class), sorted by squared distance from a point (nearest first). The positions come
// from fn_7100745e24. std::__sort is the adapted copy in lib/libcxx/sort.h. Names and layouts are
// placeholders.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
#include "UnkIter.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3_710072a110 {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3_710072a110(const UnkVec3_710072a110& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkObj_710072a110;
UnkVec3_710072a110 fn_7100745e24(UnkObj_710072a110* obj);

// Both positions are temporaries of the one comparison, so they get separate stack slots. The
// distance is written out term by term: that is the order the original computes it in.
struct UnkNearer_710072a110 {
    f32 x;
    f32 y;
    f32 z;

    f32 DistSq(const UnkVec3_710072a110& p) const {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z);
    }
    bool operator()(UnkObj_710072a110* a, UnkObj_710072a110* b) const {
        return DistSq(fn_7100745e24(a)) < DistSq(fn_7100745e24(b));
    }
};

using P = UnkObj_710072a110*;
using I = UnkIter<P>;
using C = UnkNearer_710072a110;
template void libcxx_adapted::__sort<C&, I>(I, I, C&);
template unsigned std::__sort3<C&, I>(I, I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template unsigned std::__sort5<C&, I>(I, I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

void fn_710072a110(I, I, C&)
    __attribute__((alias("_ZN14libcxx_adapted6__sortIR20UnkNearer_710072a1107UnkIterIP17UnkObj_710072a110EEEvT0_S7_T_")));
unsigned fn_710072aa60(I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR20UnkNearer_710072a1107UnkIterIP17UnkObj_710072a110EEEjT0_S7_S7_T_")));
unsigned fn_710072ad30(I, I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR20UnkNearer_710072a1107UnkIterIP17UnkObj_710072a110EEEjT0_S7_S7_S7_T_")));
unsigned fn_710072af70(I, I, I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR20UnkNearer_710072a1107UnkIterIP17UnkObj_710072a110EEEjT0_S7_S7_S7_S7_T_")));
bool fn_710072b250(I, I, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR20UnkNearer_710072a1107UnkIterIP17UnkObj_710072a110EEEbT0_S7_T_")));
