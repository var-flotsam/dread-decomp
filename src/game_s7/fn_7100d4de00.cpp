// libc++ std::sort helpers instantiated for 0x48-byte entries holding two handles and 13 words,
// sorted by the float at +0x38 in ascending order. The swaps are inlined in the game's copy despite
// their size, so the entry has its own always-inline swap with std::swap's steps. Names and layouts
// are placeholders.
#include <algorithm>
#include "types.h"

struct UnkRefCounted_7100d4de00 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_7100d4de00 {
    UnkRefCounted_7100d4de00* ptr;

    UnkHandleA_7100d4de00(const UnkHandleA_7100d4de00& o) {
        ptr = nullptr;
        UnkRefCounted_7100d4de00* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100d4de00* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_7100d4de00& operator=(const UnkHandleA_7100d4de00& o);  // fn_71002a5d1c
    ~UnkHandleA_7100d4de00() {
        UnkRefCounted_7100d4de00* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_7100d4de00 {
    union {
        UnkRefCounted_7100d4de00* ptr;
    };

    UnkHandleB_7100d4de00(const UnkHandleB_7100d4de00& o) {
        ptr = nullptr;
        UnkRefCounted_7100d4de00* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_7100d4de00* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_7100d4de00& operator=(const UnkHandleB_7100d4de00& o);  // fn_71002a5d1c
    ~UnkHandleB_7100d4de00() {
        UnkRefCounted_7100d4de00* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkEntry_7100d4de00 {
    UnkHandleA_7100d4de00 a;
    UnkHandleB_7100d4de00 b;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    union {
        f32 key;
        u32 key_bits;
    };
    u32 field_0x3c;
    u32 field_0x40;

#define UNK_COPY_7100d4de00(o)                                                                     \
    field_0x10 = o.field_0x10;                                                                     \
    field_0x14 = o.field_0x14;                                                                     \
    field_0x18 = o.field_0x18;                                                                     \
    field_0x1c = o.field_0x1c;                                                                     \
    field_0x20 = o.field_0x20;                                                                     \
    field_0x24 = o.field_0x24;                                                                     \
    field_0x28 = o.field_0x28;                                                                     \
    field_0x2c = o.field_0x2c;                                                                     \
    field_0x30 = o.field_0x30;                                                                     \
    field_0x34 = o.field_0x34;                                                                     \
    key_bits = o.key_bits;                                                                         \
    field_0x3c = o.field_0x3c;                                                                     \
    field_0x40 = o.field_0x40;

    UnkEntry_7100d4de00(const UnkEntry_7100d4de00& o) : a(o.a), b(o.b) { UNK_COPY_7100d4de00(o) }
    UnkEntry_7100d4de00& operator=(const UnkEntry_7100d4de00& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_7100d4de00(o)
        }
        return *this;
    }
#undef UNK_COPY_7100d4de00
};

// std::swap's steps, always inlined (the game's copy inlines it despite its size).
__attribute__((always_inline)) inline void swap(UnkEntry_7100d4de00& x, UnkEntry_7100d4de00& y) {
    UnkEntry_7100d4de00 t(x);
    x = y;
    y = t;
}

namespace {
struct UnkCmp_7100d4de00 {
    bool operator()(const UnkEntry_7100d4de00& x, const UnkEntry_7100d4de00& y) const {
        return x.key < y.key;
    }
};}  // namespace


using E = UnkEntry_7100d4de00;
using C = UnkCmp_7100d4de00;

// Not in the game (its caller is fn_7100d4d054): a direct call, so that the internal
// instantiations are emitted and their unused comparator arguments dropped, as in the game.
void unk_anchor_7100d4de00(E* first, E* last) {
    std::sort(first, last, C());
}

// The instantiations are internal, as in the game, so their unused comparator arguments are
// dropped; an alias would take their addresses and keep the arguments, so the names are set in
// assembly instead. The declarations below only carry the annotations (on two lines, so that
// tools/check.py reads them as definitions).
asm(".globl fn_7100d4de00\n.set fn_7100d4de00, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7100d4de00EP19UnkEntry_7100d4de00EEvT0_S6_T_");
asm(".globl fn_7100d4f130\n.set fn_7100d4f130, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_7100d4de00EP19UnkEntry_7100d4de00EEjT0_S6_S6_T_");
asm(".globl fn_7100d4fc80\n.set fn_7100d4fc80, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7100d4de00EP19UnkEntry_7100d4de00EEjT0_S6_S6_S6_T_");
asm(".globl fn_7100d503a0\n.set fn_7100d503a0, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_7100d4de00EP19UnkEntry_7100d4de00EEjT0_S6_S6_S6_S6_T_");
asm(".globl fn_7100d50d00\n.set fn_7100d50d00, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7100d4de00EP19UnkEntry_7100d4de00EEbT0_S6_T_");
extern "C" {
// NON_MATCHING(minor): libc++ 9.0.1's std::__sort; the original keeps the division's magic
// constant in x28 through the loop, so every register after it differs. Same behaviour.
void fn_7100d4de00()
    ;
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
void fn_7100d4f130()
    ;
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
void fn_7100d4fc80()
    ;
// NON_MATCHING(minor): the same operations, but a swap's temporary and handles get other
// registers, so fewer tails are merged. Same behaviour.
void fn_7100d503a0()
    ;
}
