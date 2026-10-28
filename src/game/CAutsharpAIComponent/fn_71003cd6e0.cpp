// libc++ std::sort helpers instantiated for 0x48-byte entries holding two handles and 13 words,
// sorted by direction from a point: an entry whose direction (from the comparator's position)
// lies further along the comparator's axis comes later, and entries within 1e-5 of each other
// come nearest first. The comparator is out of line (fn_71003cd4fc). The swaps are inlined in the
// game's copy despite their size, so the entry has its own always-inline swap with std::swap's
// steps. Names and layouts are placeholders.
#include <algorithm>
#include <cmath>
#include "types.h"

struct UnkRefCounted_71003cd6e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_71003cd6e0 {
    UnkRefCounted_71003cd6e0* ptr;

    UnkHandleA_71003cd6e0(const UnkHandleA_71003cd6e0& o) {
        ptr = nullptr;
        UnkRefCounted_71003cd6e0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_71003cd6e0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_71003cd6e0& operator=(const UnkHandleA_71003cd6e0& o);  // fn_71002a5d1c
    ~UnkHandleA_71003cd6e0() {
        UnkRefCounted_71003cd6e0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_71003cd6e0 {
    union {
        UnkRefCounted_71003cd6e0* ptr;
    };

    UnkHandleB_71003cd6e0(const UnkHandleB_71003cd6e0& o) {
        ptr = nullptr;
        UnkRefCounted_71003cd6e0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_71003cd6e0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_71003cd6e0& operator=(const UnkHandleB_71003cd6e0& o);  // fn_71002a5d1c
    ~UnkHandleB_71003cd6e0() {
        UnkRefCounted_71003cd6e0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkVec3_71003cd6e0 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_71003cd6e0 operator-(const UnkVec3_71003cd6e0& o) const {
        UnkVec3_71003cd6e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    UnkVec3_71003cd6e0 operator*(f32 s) const {
        UnkVec3_71003cd6e0 r;
        r.x = x * s;
        r.y = y * s;
        r.z = z * s;
        return r;
    }
    f32 Dot(const UnkVec3_71003cd6e0& o) const { return x * o.x + y * o.y + z * o.z; }
    f32 LengthSq() const { return x * x + y * y + z * z; }
    f32 Length() const { return sqrtf(LengthSq()); }
};

struct UnkEntry_71003cd6e0 {
    UnkHandleA_71003cd6e0 a;
    UnkHandleB_71003cd6e0 b;
    u32 field_0x10;
    UnkVec3_71003cd6e0 pos;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;

#define UNK_COPY_71003cd6e0(o)                                                                     \
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

    UnkEntry_71003cd6e0(const UnkEntry_71003cd6e0& o) : a(o.a), b(o.b) { UNK_COPY_71003cd6e0(o) }
    UnkEntry_71003cd6e0& operator=(const UnkEntry_71003cd6e0& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_71003cd6e0(o)
        }
        return *this;
    }
#undef UNK_COPY_71003cd6e0
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_71003cd6e0& x, UnkEntry_71003cd6e0& y) {
    UnkEntry_71003cd6e0 t(x);
    x = y;
    y = t;
}

struct UnkCmp_71003cd6e0 {
    UnkVec3_71003cd6e0 pos;
    UnkVec3_71003cd6e0 dir;

    // Returns a word, not a bool: the callers test all of w0 (cbz), not bit 0.
    s32 operator()(const UnkEntry_71003cd6e0& a, const UnkEntry_71003cd6e0& b) const;

    // Cosine of the angle between the axis and the direction to the entry.
    f32 Along(const UnkEntry_71003cd6e0& e) const {
        UnkVec3_71003cd6e0 d = e.pos - pos;
        return dir.Dot(d * (1.0f / (d.Length() + 0.0001f)));
    }
    f32 DistSq(const UnkEntry_71003cd6e0& e) const {
        return (e.pos.x - pos.x) * (e.pos.x - pos.x) + (e.pos.y - pos.y) * (e.pos.y - pos.y) +
               (e.pos.z - pos.z) * (e.pos.z - pos.z);
    }
};

s32 UnkCmp_71003cd6e0::operator()(const UnkEntry_71003cd6e0& a, const UnkEntry_71003cd6e0& b) const {
    f32 along_a = Along(a);
    f32 along_b = Along(b);
    f32 lo = along_a - 1e-05f;
    f32 hi = along_a + 1e-05f;
    // Both bounds are computed before either test (a short-circuit && sinks the lower one).
    if ((hi >= along_b) & (lo <= along_b))
        return DistSq(a) < DistSq(b);
    return along_a < along_b;
}

using E = UnkEntry_71003cd6e0;
using C = UnkCmp_71003cd6e0;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);

// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the first and last pointers swap registers
// (x20/x21) and the spilled values take other stack slots. Same behaviour.
void fn_71003cd6e0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEvT0_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_71003ce800(E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_71003cf320(E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_71003cfa30(E*, E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEjT0_S5_S5_S5_S5_T_")));
void fn_71003d0390(E*, E*, C&)
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEvT0_S5_T_")));
bool fn_71003d0670(E*, E*, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_71003cd6e0P19UnkEntry_71003cd6e0EEbT0_S5_T_")));
s32 fn_71003cd4fc(const UnkCmp_71003cd6e0*, const E&, const E&)
    __attribute__((alias("_ZNK17UnkCmp_71003cd6e0clERK19UnkEntry_71003cd6e0S2_")));
