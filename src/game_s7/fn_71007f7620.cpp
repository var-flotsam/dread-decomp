// libc++ std::sort helpers instantiated for a container of object pointers (through its own
// iterator class), sorted by the float at +0x4c in ascending order. The comparator is internal,
// so the helpers take no comparator argument. Names and layouts are placeholders.
#include <algorithm>
#include "types.h"
#include "UnkIter.h"

struct UnkObj_71007f7620 {
    u8 pad_0x0[0x4c];
    // In a union: the original reloads it after each store of an element.
    union {
        f32 field_0x4c;
    };
};

namespace {

struct UnkCmp_71007f7620 {
    bool operator()(const UnkObj_71007f7620* a, const UnkObj_71007f7620* b) const {
        return a->field_0x4c < b->field_0x4c;
    }
};

}  // namespace

using I = UnkIter<UnkObj_71007f7620*>;
using C = UnkCmp_71007f7620;

// Not in the game: a direct call, so that the internal instantiations are emitted and their
// unused comparator arguments dropped, as in the game.
void unk_anchor_71007f7620(I first, I last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_71007f7620\n.set fn_71007f7620, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_71007f7620E7UnkIterIP17UnkObj_71007f7620EEEvT0_S8_T_");
asm(".globl fn_71007f7c70\n.set fn_71007f7c70, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71007f7620E7UnkIterIP17UnkObj_71007f7620EEEjT0_S8_S8_S8_T_");
asm(".globl fn_71007f7dd0\n.set fn_71007f7dd0, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71007f7620E7UnkIterIP17UnkObj_71007f7620EEEbT0_S8_T_");
