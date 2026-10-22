// libc++ std::sort helpers instantiated for a list of object pointers, ordered by a float in each
// object's info block. The comparator copies both blocks into default-constructed temporaries
// before comparing them. Names and layouts are placeholders.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
#include "types.h"

// Default-constructed out of line (fn_710021685c); its virtual destructor is fn_7100216934.
struct UnkInfo_71001e2e30 {
    UnkInfo_71001e2e30();
    virtual ~UnkInfo_71001e2e30();

    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 key;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u64 field_0x38;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u8 field_0x4c;
    u8 field_0x4d;

    // Leaves field_0xc to field_0x14 alone.
    UnkInfo_71001e2e30& operator=(const UnkInfo_71001e2e30& o) {
        field_0x8 = o.field_0x8;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        key = o.key;
        field_0x24 = o.field_0x24;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x30 = o.field_0x30;
        field_0x38 = o.field_0x38;
        field_0x40 = o.field_0x40;
        field_0x44 = o.field_0x44;
        field_0x48 = o.field_0x48;
        field_0x4c = o.field_0x4c;
        field_0x4d = o.field_0x4d;
        return *this;
    }
};

struct UnkThing_71001e2e30 {
    u8 pad_0x0[0x20];
    UnkInfo_71001e2e30 info;
};

struct UnkObj_71001e2e30 {
    u8 pad_0x0[0x10];
    UnkThing_71001e2e30* field_0x10;
};

namespace {

struct UnkCmp_71001e2e30 {
    // Declared inline: the hint raises the inlining threshold, which the comparator needs to be
    // inlined into every helper as in the game.
    inline bool operator()(const UnkObj_71001e2e30* a, const UnkObj_71001e2e30* b) const {
        // Both temporaries are constructed before either is assigned.
        UnkInfo_71001e2e30 ia;
        UnkInfo_71001e2e30 ib;
        ia = a->field_0x10->info;
        ib = b->field_0x10->info;
        return ia.key < ib.key;
    }
};

}  // namespace

using P = UnkObj_71001e2e30*;
using C = UnkCmp_71001e2e30;

// Not in the game: a direct call, so that the internal instantiations are emitted and their
// unused comparator arguments dropped, as in the game.
void unk_anchor_71001e2e30(P* first, P* last) {
    C c;
    libcxx_adapted::__sort<C&>(first, last, c);
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments). The declarations below only carry the annotations.
asm(".globl fn_71001e2e30\n.set fn_71001e2e30, _ZN14libcxx_adapted6__sortIRN12_GLOBAL__N_117UnkCmp_71001e2e30EPP17UnkObj_71001e2e30EEvT0_S7_T_");
asm(".globl fn_71001e3df0\n.set fn_71001e3df0, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_71001e2e30EPP17UnkObj_71001e2e30EEjT0_S7_S7_T_");
asm(".globl fn_71001e4310\n.set fn_71001e4310, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71001e2e30EPP17UnkObj_71001e2e30EEjT0_S7_S7_S7_T_");
asm(".globl fn_71001e4700\n.set fn_71001e4700, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_71001e2e30EPP17UnkObj_71001e2e30EEjT0_S7_S7_S7_S7_T_");
asm(".globl fn_71001e4c20\n.set fn_71001e4c20, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71001e2e30EPP17UnkObj_71001e2e30EEbT0_S7_T_");
extern "C" {
// NON_MATCHING(minor): the adapted std::__sort; the inlined insertion sort walks the elements with
// another pointer, and other values move between registers and the stack. Same behaviour.
void fn_71001e2e30()
    ;
// NON_MATCHING(minor): the insertion loop walks an index where the original walks a pointer; same
// behaviour.
void fn_71001e4c20()
    ;
}
