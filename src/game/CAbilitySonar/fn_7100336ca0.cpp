// libc++ std::sort helpers instantiated for a list of weak handles (through its own iterator
// class), sorted by squared distance from a target point to each object's position (the
// translation of a 3x4 matrix), nearest first. The handle is assigned out of line
// (fn_71003384e0). The comparator has internal linkage, so every instantiation is internal and
// std::__sort inlines __insertion_sort_3 as the game's copy does. Names and layouts are
// placeholders.
//
// None match yet: the operations and sizes are the same, but the comparisons load the target and
// the positions in another order (the original starts with the first handle's chain; ours with the
// target), so the registers differ. No way of writing the distance changed that. Same behaviour.
#include <algorithm>
#include "../../game_s7/UnkIter.h"
#include "types.h"

struct UnkVec3_7100336ca0 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkTransform_7100336ca0 {
    u8 unk_0x0[0x38];
    f32 mtx[3][4];
};

struct UnkObj_7100336ca0 {
    u8 unk_0x0[0x18];
    UnkTransform_7100336ca0* transform;
};

struct UnkRefCounted_7100336ca0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;
    UnkObj_7100336ca0* obj;
};

// A weak handle: assigned out of line (fn_71003384e0), copied by assigning to an empty one.
struct UnkHandle_7100336ca0 {
    UnkRefCounted_7100336ca0* ptr;

    UnkHandle_7100336ca0(const UnkHandle_7100336ca0& o) {
        ptr = nullptr;
        UnkRefCounted_7100336ca0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100336ca0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandle_7100336ca0& operator=(const UnkHandle_7100336ca0& o);  // fn_71003384e0
    ~UnkHandle_7100336ca0() {
        UnkRefCounted_7100336ca0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// std::swap's steps, always inlined (the game's copy inlines it).
__attribute__((always_inline)) inline void swap(UnkHandle_7100336ca0& x, UnkHandle_7100336ca0& y) {
    UnkHandle_7100336ca0 t(x);
    x = y;
    y = t;
}

namespace {
struct UnkNearer_7100336ca0 {
    const UnkVec3_7100336ca0* target;

    static UnkVec3_7100336ca0 Pos(const UnkHandle_7100336ca0& h) {
        const UnkTransform_7100336ca0* t = h.ptr->obj->transform;
        UnkVec3_7100336ca0 p = {t->mtx[0][3], t->mtx[1][3], t->mtx[2][3]};
        return p;
    }
    f32 DistSq(const UnkVec3_7100336ca0& p) const {
        return (target->x - p.x) * (target->x - p.x) + (target->y - p.y) * (target->y - p.y) +
               (target->z - p.z) * (target->z - p.z);
    }
    bool operator()(const UnkHandle_7100336ca0& a, const UnkHandle_7100336ca0& b) const {
        return DistSq(Pos(a)) < DistSq(Pos(b));
    }
};
}  // namespace

using H0 = UnkHandle_7100336ca0;
using H = UnkIter<H0>;
using C = UnkNearer_7100336ca0;
template void std::__sort<C&, H>(H, H, C&);
template unsigned std::__sort3<C&, H>(H, H, H, C&);
template unsigned std::__sort4<C&, H>(H, H, H, H, C&);
template unsigned std::__sort5<C&, H>(H, H, H, H, H, C&);
template bool std::__insertion_sort_incomplete<C&, H>(H, H, C&);

// extern "C", so the names stay plain.
extern "C" {
// NON_MATCHING(minor): load order in the comparisons, see the note at the top of the file.
void fn_7100336ca0()
    __attribute__((alias("_ZNSt3__16__sortIRN12_GLOBAL__N_120UnkNearer_7100336ca0E7UnkIterI20UnkHandle_7100336ca0EEEvT0_S7_T_")));
// NON_MATCHING(minor): load order in the comparisons, see the note at the top of the file.
void fn_71003376b0()
    __attribute__((alias("_ZNSt3__17__sort3IRN12_GLOBAL__N_120UnkNearer_7100336ca0E7UnkIterI20UnkHandle_7100336ca0EEEjT0_S7_S7_T_")));
// NON_MATCHING(minor): load order in the comparisons, see the note at the top of the file.
void fn_7100337a80()
    __attribute__((alias("_ZNSt3__17__sort4IRN12_GLOBAL__N_120UnkNearer_7100336ca0E7UnkIterI20UnkHandle_7100336ca0EEEjT0_S7_S7_S7_T_")));
// NON_MATCHING(minor): load order in the comparisons, see the note at the top of the file.
void fn_7100337d80()
    __attribute__((alias("_ZNSt3__17__sort5IRN12_GLOBAL__N_120UnkNearer_7100336ca0E7UnkIterI20UnkHandle_7100336ca0EEEjT0_S7_S7_S7_S7_T_")));
// NON_MATCHING(minor): load order in the comparisons, see the note at the top of the file.
void fn_7100338170()
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_120UnkNearer_7100336ca0E7UnkIterI20UnkHandle_7100336ca0EEEbT0_S7_T_")));
}
