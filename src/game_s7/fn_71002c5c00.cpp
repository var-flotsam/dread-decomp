// libc++ std::sort helpers instantiated for a container of object pointers (through its own
// iterator class), sorted by the float at +0x38 in descending order. The comparator is internal,
// so the helpers take no comparator argument. Names and layouts are placeholders.
#include <algorithm>
#include "types.h"
#include "UnkIter.h"

struct UnkObj_71002c5c00 {
    u8 pad_0x0[0x38];
    // In a union: the original reloads it after each store of an element.
    union {
        f32 field_0x38;
    };
};

namespace {

struct UnkCmp_71002c5c00 {
    bool operator()(const UnkObj_71002c5c00* a, const UnkObj_71002c5c00* b) const {
        return a->field_0x38 > b->field_0x38;
    }
};

}  // namespace

using I = UnkIter<UnkObj_71002c5c00*>;
using C = UnkCmp_71002c5c00;

// Not in the game: a direct call, so that the internal instantiations are emitted and their
// unused comparator arguments dropped, as in the game.
void unk_anchor_71002c5c00(I first, I last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_71002c5c00\n.set fn_71002c5c00, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_71002c5c00E7UnkIterIP17UnkObj_71002c5c00EEEvT0_S8_T_");
asm(".globl fn_71002c6250\n.set fn_71002c6250, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71002c5c00E7UnkIterIP17UnkObj_71002c5c00EEEjT0_S8_S8_S8_T_");
asm(".globl fn_71002c63b0\n.set fn_71002c63b0, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71002c5c00E7UnkIterIP17UnkObj_71002c5c00EEEbT0_S8_T_");
