// libc++ std::sort helpers instantiated for 0x48-byte entries holding two handles and 13 words,
// sorted nearest first by distance from the comparator's point, with each entry's position taken
// at z = 0. Entries within 0.1 (squared distance) of each other put one whose object matches the
// comparator's mask first. The caller is fn_710081c834. The swaps are inlined in the game's copy
// despite their size, so the entry has its own always-inline swap with std::swap's steps. Names
// and layouts are placeholders.
#include <algorithm>
#include "types.h"

struct UnkObj_710081ccd0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
};

struct UnkRefCounted_710081ccd0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
    UnkObj_710081ccd0* field_0x10;
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_710081ccd0 {
    UnkRefCounted_710081ccd0* ptr;

    UnkHandleA_710081ccd0(const UnkHandleA_710081ccd0& o) {
        ptr = nullptr;
        UnkRefCounted_710081ccd0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710081ccd0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_710081ccd0& operator=(const UnkHandleA_710081ccd0& o);  // fn_71002a5d1c
    ~UnkHandleA_710081ccd0() {
        UnkRefCounted_710081ccd0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_710081ccd0 {
    union {
        UnkRefCounted_710081ccd0* ptr;
    };

    UnkHandleB_710081ccd0(const UnkHandleB_710081ccd0& o) {
        ptr = nullptr;
        UnkRefCounted_710081ccd0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710081ccd0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_710081ccd0& operator=(const UnkHandleB_710081ccd0& o);  // fn_71002a5d1c
    ~UnkHandleB_710081ccd0() {
        UnkRefCounted_710081ccd0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkEntry_710081ccd0 {
    UnkHandleA_710081ccd0 a;
    UnkHandleB_710081ccd0 b;
    u32 field_0x10;
    f32 x;
    f32 y;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;

#define UNK_COPY_710081ccd0(o)                                                                     \
    field_0x10 = o.field_0x10;                                                                     \
    x = o.x;                                                                     \
    y = o.y;                                                                                       \
    field_0x1c = o.field_0x1c;                                                                     \
    field_0x20 = o.field_0x20;                                                                     \
    field_0x24 = o.field_0x24;                                                                     \
    field_0x28 = o.field_0x28;                                                                     \
    field_0x2c = o.field_0x2c;                                                                     \
    field_0x30 = o.field_0x30;                                                                     \
    field_0x34 = o.field_0x34;                                                                     \
    field_0x38 = o.field_0x38;                                                                     \
    field_0x3c = o.field_0x3c;                                                                     \
    field_0x40 = o.field_0x40;

    UnkEntry_710081ccd0(const UnkEntry_710081ccd0& o) : a(o.a), b(o.b) { UNK_COPY_710081ccd0(o) }
    UnkEntry_710081ccd0& operator=(const UnkEntry_710081ccd0& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_710081ccd0(o)
        }
        return *this;
    }
#undef UNK_COPY_710081ccd0
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_710081ccd0& x, UnkEntry_710081ccd0& y) {
    UnkEntry_710081ccd0 t(x);
    x = y;
    y = t;
}

struct UnkMask_710081ccd0 {
    s32 count;
    u32 words[4];  // the caller copies count words
};

static inline bool Near_710081ccd0(const f32& x, const f32& y) {
    return x - 0.1f <= y && x + 0.1f >= y;
}

struct UnkVec3_710081ccd0 {
    f32 x;
    f32 y;
    f32 z;
};

static inline f32 DistSq_710081ccd0(const UnkVec3_710081ccd0& p, const UnkVec3_710081ccd0& q) {
    return (p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y) + (p.z - q.z) * (p.z - q.z);
}

struct UnkCmp_710081ccd0 {
    UnkVec3_710081ccd0 pos;
    UnkMask_710081ccd0 mask;

    // A member, not DistSq_710081ccd0: that keeps the order the original loads and computes in.
    f32 DistSq(const UnkVec3_710081ccd0& p) const {
        return (pos.x - p.x) * (pos.x - p.x) + (pos.y - p.y) * (pos.y - p.y) +
               (pos.z - p.z) * (pos.z - p.z);
    }

    bool Has(const UnkEntry_710081ccd0& e) const {
        return (e.a.ptr->field_0x10->vf_0x58() & mask.words[0]) != 0;
    }
    bool operator()(const UnkEntry_710081ccd0& a, const UnkEntry_710081ccd0& b) const {
        UnkVec3_710081ccd0 pb = {b.x, b.y, 0.0f};
        f32 db = DistSq(pb);
        UnkVec3_710081ccd0 pa = {a.x, a.y, 0.0f};
        f32 da = DistSq(pa);
        f32 dab = DistSq_710081ccd0(pb, pa);
        if (Near_710081ccd0(dab, 0.0f)) {
            bool ha = Has(a);
            bool hb = Has(b);
            if (ha && !hb)
                return true;
            if (!ha && hb)
                return false;
        }
        return da < db;
    }
};

using E = UnkEntry_710081ccd0;
using C = UnkCmp_710081ccd0;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);

// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the comparator's two constants swap registers,
// and the swaps' temporaries get other registers, so fewer tails are merged. Same behaviour.
void fn_710081ccd0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEvT0_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710081e620(E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710081f4f0(E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710081fec0(E*, E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEjT0_S5_S5_S5_S5_T_")));
void fn_7100820bc0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEvT0_S5_T_")));
bool fn_7100821080(E*, E*, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_710081ccd0P19UnkEntry_710081ccd0EEbT0_S5_T_")));
