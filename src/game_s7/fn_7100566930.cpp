// libc++ std::sort helpers instantiated for 0x48-byte entries holding two handles and 13 words,
// sorted by the float at +0x18 in ascending order. The swaps are inlined in the game's copy despite
// their size, so the entry has its own always-inline swap with std::swap's steps. Names and layouts
// are placeholders.
#include <algorithm>
#include "types.h"

struct UnkRefCounted_7100566930 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_7100566930 {
    UnkRefCounted_7100566930* ptr;

    UnkHandleA_7100566930(const UnkHandleA_7100566930& o) {
        ptr = nullptr;
        UnkRefCounted_7100566930* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100566930* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_7100566930& operator=(const UnkHandleA_7100566930& o);  // fn_71002a5d1c
    ~UnkHandleA_7100566930() {
        UnkRefCounted_7100566930* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_7100566930 {
    union {
        UnkRefCounted_7100566930* ptr;
    };

    UnkHandleB_7100566930(const UnkHandleB_7100566930& o) {
        ptr = nullptr;
        UnkRefCounted_7100566930* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100566930* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_7100566930& operator=(const UnkHandleB_7100566930& o);  // fn_71002a5d1c
    ~UnkHandleB_7100566930() {
        UnkRefCounted_7100566930* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkEntry_7100566930 {
    UnkHandleA_7100566930 a;
    UnkHandleB_7100566930 b;
    u32 field_0x10;
    u32 field_0x14;
    union {
        f32 key;
        u32 key_bits;
    };
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

#define UNK_COPY_7100566930(o)                                                                     \
    field_0x10 = o.field_0x10;                                                                     \
    field_0x14 = o.field_0x14;                                                                     \
    key_bits = o.key_bits;                                                                         \
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

    UnkEntry_7100566930(const UnkEntry_7100566930& o) : a(o.a), b(o.b) { UNK_COPY_7100566930(o) }
    UnkEntry_7100566930& operator=(const UnkEntry_7100566930& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_7100566930(o)
        }
        return *this;
    }
#undef UNK_COPY_7100566930
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_7100566930& x, UnkEntry_7100566930& y) {
    UnkEntry_7100566930 t(x);
    x = y;
    y = t;
}

struct UnkCmp_7100566930 {
    bool operator()(const UnkEntry_7100566930& x, const UnkEntry_7100566930& y) const {
        return x.key < y.key;
    }
};

using E = UnkEntry_7100566930;
using C = UnkCmp_7100566930;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);

// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the game's copy lays the blocks out in another
// order (as fn_710117c430 does) and halves lengths as unsigned. Same behaviour.
void fn_7100566930(E*, E*, C&)
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_7100566930P19UnkEntry_7100566930EEvT0_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_71005679f0(E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_7100566930P19UnkEntry_7100566930EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_7100568540(E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_7100566930P19UnkEntry_7100566930EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_7100568c60(E*, E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_7100566930P19UnkEntry_7100566930EEjT0_S5_S5_S5_S5_T_")));
void fn_71005695c0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_7100566930P19UnkEntry_7100566930EEvT0_S5_T_")));
bool fn_7100569890(E*, E*, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_7100566930P19UnkEntry_7100566930EEbT0_S5_T_")));
