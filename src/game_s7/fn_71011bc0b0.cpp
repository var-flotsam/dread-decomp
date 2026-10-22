// libc++ std::sort helpers instantiated for a list of object pointers (through its own iterator
// class), largest key first: the s32 at +0x140, or the one at +0x13c while that is INT32_MAX. The
// field at +0x140 is a union member, so the inserted element's key is read again after each store
// as in the game. Names and layouts are placeholders.
#include <algorithm>
#include "UnkIter.h"
#include "types.h"

struct UnkObj_71011bc0b0 {
    u8 pad_0x0[0x13c];
    s32 field_0x13c;
    union {
        s32 field_0x140;
    };

    s32 Key() const { return field_0x140 != 0x7fffffff ? field_0x140 : field_0x13c; }
};

namespace {

struct UnkCmp_71011bc0b0 {
    bool operator()(const UnkObj_71011bc0b0* a, const UnkObj_71011bc0b0* b) const {
        return a->Key() > b->Key();
    }
};

}  // namespace

using P = UnkIter<UnkObj_71011bc0b0*>;
using C = UnkCmp_71011bc0b0;

// Not in the game: a direct call, so that the internal instantiations are emitted and their unused
// comparator arguments dropped, as in the game.
void unk_anchor_71011bc0b0(P first, P last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_71011bc0b0\n.set fn_71011bc0b0, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_71011bc0b0E7UnkIterIP17UnkObj_71011bc0b0EEEvT0_S8_T_");
asm(".globl fn_71011bc830\n.set fn_71011bc830, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_71011bc0b0E7UnkIterIP17UnkObj_71011bc0b0EEEjT0_S8_S8_T_");
asm(".globl fn_71011bc9b0\n.set fn_71011bc9b0, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71011bc0b0E7UnkIterIP17UnkObj_71011bc0b0EEEjT0_S8_S8_S8_T_");
asm(".globl fn_71011bcb20\n.set fn_71011bcb20, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_71011bc0b0E7UnkIterIP17UnkObj_71011bc0b0EEEjT0_S8_S8_S8_S8_T_");
asm(".globl fn_71011bcd00\n.set fn_71011bcd00, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71011bc0b0E7UnkIterIP17UnkObj_71011bc0b0EEEbT0_S8_T_");
