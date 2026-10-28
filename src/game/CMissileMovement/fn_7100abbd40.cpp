// libc++ std::sort helpers instantiated for 0x48-byte entries holding two handles and 13 words,
// nearest first: by the squared distance from the comparator's point to the entry's position
// with z taken as 0, compared through the sign of the difference. The same code as
// fn_7100354e10's family, instantiated again for other types. The swaps are inlined in the game's
// copy despite their size, so the entry has its own always-inline swap with std::swap's steps.
// Names and layouts are placeholders.
#include <algorithm>
#include "types.h"

struct UnkRefCounted_7100abbd40 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_7100abbd40 {
    UnkRefCounted_7100abbd40* ptr;

    UnkHandleA_7100abbd40(const UnkHandleA_7100abbd40& o) {
        ptr = nullptr;
        UnkRefCounted_7100abbd40* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100abbd40* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_7100abbd40& operator=(const UnkHandleA_7100abbd40& o);  // fn_71002a5d1c
    ~UnkHandleA_7100abbd40() {
        UnkRefCounted_7100abbd40* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_7100abbd40 {
    union {
        UnkRefCounted_7100abbd40* ptr;
    };

    UnkHandleB_7100abbd40(const UnkHandleB_7100abbd40& o) {
        ptr = nullptr;
        UnkRefCounted_7100abbd40* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100abbd40* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_7100abbd40& operator=(const UnkHandleB_7100abbd40& o);  // fn_71002a5d1c
    ~UnkHandleB_7100abbd40() {
        UnkRefCounted_7100abbd40* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkVec3_7100abbd40 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_7100abbd40 operator-(const UnkVec3_7100abbd40& o) const {
        UnkVec3_7100abbd40 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    f32 LengthSq() const { return x * x + y * y + z * z; }
};

struct UnkEntry_7100abbd40 {
    UnkHandleA_7100abbd40 a;
    UnkHandleB_7100abbd40 b;
    u32 field_0x10;
    UnkVec3_7100abbd40 pos;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;

#define UNK_COPY_7100abbd40(o)                                                                     \
    field_0x10 = o.field_0x10;                                                                     \
    pos.x = o.pos.x;                                                                               \
    pos.y = o.pos.y;                                                                               \
    pos.z = o.pos.z;                                                                               \
    field_0x20 = o.field_0x20;                                                                     \
    field_0x24 = o.field_0x24;                                                                     \
    field_0x28 = o.field_0x28;                                                                     \
    field_0x2c = o.field_0x2c;                                                                     \
    field_0x30 = o.field_0x30;                                                                     \
    field_0x34 = o.field_0x34;                                                                     \
    field_0x38 = o.field_0x38;                                                                     \
    field_0x3c = o.field_0x3c;                                                                     \
    field_0x40 = o.field_0x40;

    UnkEntry_7100abbd40(const UnkEntry_7100abbd40& o) : a(o.a), b(o.b) { UNK_COPY_7100abbd40(o) }
    UnkEntry_7100abbd40& operator=(const UnkEntry_7100abbd40& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_7100abbd40(o)
        }
        return *this;
    }
#undef UNK_COPY_7100abbd40
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_7100abbd40& x, UnkEntry_7100abbd40& y) {
    UnkEntry_7100abbd40 t(x);
    x = y;
    y = t;
}

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

struct UnkCmp_7100abbd40 {
    UnkVec3_7100abbd40 pos;

    // Each position is copied with z = 0 before the distance is taken; that keeps the order the
    // original loads and computes in.
    f32 DistSq(const UnkVec3_7100abbd40& p) const {
        return (pos.x - p.x) * (pos.x - p.x) + (pos.y - p.y) * (pos.y - p.y) +
               (pos.z - p.z) * (pos.z - p.z);
    }
    bool operator()(const UnkEntry_7100abbd40& a, const UnkEntry_7100abbd40& b) const {
        UnkVec3_7100abbd40 pb = {b.pos.x, b.pos.y, 0.0f};
        f32 db = DistSq(pb);
        UnkVec3_7100abbd40 pa = {a.pos.x, a.pos.y, 0.0f};
        f32 da = DistSq(pa);
        return (s32)Sign(db - da) > 0;
    }
};

using E = UnkEntry_7100abbd40;
using C = UnkCmp_7100abbd40;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);

// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the first and last pointers swap registers
// (x20/x21) and the spilled values take other stack slots. Same behaviour.
void fn_7100abbd40(E*, E*, C&)
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEvT0_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_7100abd0f0(E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_7100abdd40(E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_7100abe550(E*, E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEjT0_S5_S5_S5_S5_T_")));
void fn_7100abf000(E*, E*, C&)
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEvT0_S5_T_")));
bool fn_7100abf390(E*, E*, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_7100abbd40P19UnkEntry_7100abbd40EEbT0_S5_T_")));
