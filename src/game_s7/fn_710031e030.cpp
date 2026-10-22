// libc++ std::sort helpers instantiated for 0x30-byte entries holding two handles and plain data,
// sorted by the float at +0x24 in ascending order. The first handle is assigned out of line by
// fn_710031d208, the second by fn_71002a5d1c; the plain members are copied as one block, as an
// implicit copy does. The swaps are inlined in the game's copy despite their size, so the entry has
// its own always-inline swap with std::swap's steps. Names and layouts are placeholders.
#include <algorithm>
#include "types.h"

struct UnkRefCounted_710031e030 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A handle: assigned out of line (fn_710031d208), copied by assigning to an empty one.
struct UnkHandleA_710031e030 {
    UnkRefCounted_710031e030* ptr;

    UnkHandleA_710031e030(const UnkHandleA_710031e030& o) {
        ptr = nullptr;
        UnkRefCounted_710031e030* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710031e030* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_710031e030& operator=(const UnkHandleA_710031e030& o);  // fn_710031d208
    ~UnkHandleA_710031e030() {
        UnkRefCounted_710031e030* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_710031e030 {
    union {
        UnkRefCounted_710031e030* ptr;
    };

    UnkHandleB_710031e030(const UnkHandleB_710031e030& o) {
        ptr = nullptr;
        UnkRefCounted_710031e030* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710031e030* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_710031e030& operator=(const UnkHandleB_710031e030& o);  // fn_71002a5d1c
    ~UnkHandleB_710031e030() {
        UnkRefCounted_710031e030* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkEntry_710031e030 {
    UnkHandleA_710031e030 a;
    UnkHandleB_710031e030 b;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    f32 key;
    bool field_0x28;
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_710031e030& x, UnkEntry_710031e030& y) {
    UnkEntry_710031e030 t(x);
    x = y;
    y = t;
}

struct UnkCmp_710031e030 {
    bool operator()(const UnkEntry_710031e030& x, const UnkEntry_710031e030& y) const {
        return x.key < y.key;
    }
};

using E = UnkEntry_710031e030;
using C = UnkCmp_710031e030;
template void std::__sort<C&, E*>(E*, E*, C&);
template unsigned std::__sort3<C&, E*>(E*, E*, E*, C&);
template unsigned std::__sort4<C&, E*>(E*, E*, E*, E*, C&);
template unsigned std::__sort5<C&, E*>(E*, E*, E*, E*, E*, C&);
template bool std::__insertion_sort_incomplete<C&, E*>(E*, E*, C&);
template void std::__insertion_sort_3<C&, E*>(E*, E*, C&);

// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the game's copy lays the blocks out in another
// order (as fn_710117c430 does) and halves lengths as unsigned. Same behaviour.
void fn_710031e030(E*, E*, C&)
    __attribute__((alias("_ZNSt3__16__sortIR17UnkCmp_710031e030P19UnkEntry_710031e030EEvT0_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710031eaa0(E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort3IR17UnkCmp_710031e030P19UnkEntry_710031e030EEjT0_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710031f110(E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort4IR17UnkCmp_710031e030P19UnkEntry_710031e030EEjT0_S5_S5_S5_T_")));
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
unsigned fn_710031f550(E*, E*, E*, E*, E*, C&)
    __attribute__((alias("_ZNSt3__17__sort5IR17UnkCmp_710031e030P19UnkEntry_710031e030EEjT0_S5_S5_S5_S5_T_")));
void fn_710031fad0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__118__insertion_sort_3IR17UnkCmp_710031e030P19UnkEntry_710031e030EEvT0_S5_T_")));
bool fn_710031fcd0(E*, E*, C&)
    __attribute__((alias("_ZNSt3__127__insertion_sort_incompleteIR17UnkCmp_710031e030P19UnkEntry_710031e030EEbT0_S5_T_")));
