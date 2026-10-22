// libc++ std::sort helpers instantiated for a container of 0x40-byte polymorphic boxes (through its
// own iterator class), sorted by the centre's coordinate on the comparator's axis. A box copy
// rebuilds the box from its corners and stores them again as whole tenths. std::__sort is the
// adapted copy in lib/libcxx/sort.h. The box constructor (fn_7100215dcc) and destructor
// (fn_7100215e1c) are defined elsewhere. Names and layouts are placeholders.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
#include "UnkIter.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3_7100213380 {
    f32 v[3];
    UnkVec3_7100213380(const UnkVec3_7100213380& o) {
        v[0] = o.v[0];
        v[1] = o.v[1];
        v[2] = o.v[2];
    }
    f32 operator[](u32 i) const { return v[i]; }
};

// Assigned member by member.
struct UnkRange_7100213380 {
    f32 min;
    f32 max;
    UnkRange_7100213380& operator=(const UnkRange_7100213380& o) {
        min = o.min;
        max = o.max;
        return *this;
    }
};

struct UnkBox_7100213380 {
    UnkRange_7100213380 r[3];

    UnkBox_7100213380(const UnkVec3_7100213380& min, const UnkVec3_7100213380& max);
    virtual ~UnkBox_7100213380() {}
    // One inline virtual keeps the vtable out of this file, so the calls on a copy stay indirect.
    virtual void vf_0x10() {}
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual UnkVec3_7100213380 vf_0x38() const;
    virtual UnkVec3_7100213380 vf_0x40() const;
    UnkVec3_7100213380 Center() const;
};

struct UnkIBox_7100213380 {
    s32 v[6];
};

struct UnkBoxQ_7100213380 : UnkBox_7100213380 {
    UnkIBox_7100213380 q;
    u32 field_0x38;

    UnkBoxQ_7100213380(const UnkBoxQ_7100213380& o) : UnkBox_7100213380(o.vf_0x38(), o.vf_0x40()) {
        UnkVec3_7100213380 a = vf_0x38();
        UnkVec3_7100213380 b = vf_0x40();
        q.v[0] = (s32)(a[0] * 10.0f);
        q.v[1] = (s32)(a[1] * 10.0f);
        q.v[2] = (s32)(a[2] * 10.0f);
        q.v[3] = (s32)(b[0] * 10.0f);
        q.v[4] = (s32)(b[1] * 10.0f);
        q.v[5] = (s32)(b[2] * 10.0f);
        field_0x38 = o.field_0x38;
    }
    UnkBoxQ_7100213380& operator=(const UnkBoxQ_7100213380& o) {
        UnkBox_7100213380::operator=(o);
        q = o.q;
        field_0x38 = o.field_0x38;
        return *this;
    }
    ~UnkBoxQ_7100213380() override;
};

struct UnkByAxis_7100213380 {
    u32 axis;
    bool operator()(const UnkBoxQ_7100213380& a, const UnkBoxQ_7100213380& b) const {
        return a.Center()[axis] < b.Center()[axis];
    }
};

using E = UnkBoxQ_7100213380;
using I = UnkIter<E>;
using C = UnkByAxis_7100213380;
template void libcxx_adapted::__sort<C&, I>(I, I, C&);
template unsigned std::__sort3<C&, I>(I, I, I, C&);
template unsigned std::__sort4<C&, I>(I, I, I, I, C&);
template unsigned std::__sort5<C&, I>(I, I, I, I, I, C&);
template bool std::__insertion_sort_incomplete<C&, I>(I, I, C&);

void fn_7100213380(I, I, C&)
    __attribute__((alias("_ZN14libcxx_adapted6__sortIR20UnkByAxis_71002133807UnkIterI18UnkBoxQ_7100213380EEEvT0_S6_T_")));
unsigned fn_7100214230(I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR20UnkByAxis_71002133807UnkIterI18UnkBoxQ_7100213380EEEjT0_S6_S6_T_")));
unsigned fn_7100214a20(I, I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR20UnkByAxis_71002133807UnkIterI18UnkBoxQ_7100213380EEEjT0_S6_S6_S6_T_")));
unsigned fn_7100214f40(I, I, I, I, I, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR20UnkByAxis_71002133807UnkIterI18UnkBoxQ_7100213380EEEjT0_S6_S6_S6_S6_T_")));
bool fn_7100215600(I, I, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR20UnkByAxis_71002133807UnkIterI18UnkBoxQ_7100213380EEEbT0_S6_T_")));
