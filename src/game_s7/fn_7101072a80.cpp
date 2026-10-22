// libc++ std::sort helpers instantiated for a list of object pointers (through its own iterator
// class), ordered by fn_710019536c's value, largest first. Names are placeholders.
#include <algorithm>
#include "UnkIter.h"
#include "types.h"

struct UnkObj_7101072a80;
u32 fn_710019536c(UnkObj_7101072a80* obj);

namespace {

struct UnkCmp_7101072a80 {
    bool operator()(UnkObj_7101072a80* a, UnkObj_7101072a80* b) const {
        return fn_710019536c(a) > fn_710019536c(b);
    }
};

}  // namespace

using P = UnkIter<UnkObj_7101072a80*>;
using C = UnkCmp_7101072a80;

// Not in the game: a direct call, so that the internal instantiations are emitted and their unused
// comparator arguments dropped, as in the game.
void unk_anchor_7101072a80(P first, P last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_7101072a80\n.set fn_7101072a80, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7101072a80E7UnkIterIP17UnkObj_7101072a80EEEvT0_S8_T_");
asm(".globl fn_7101072f30\n.set fn_7101072f30, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_7101072a80E7UnkIterIP17UnkObj_7101072a80EEEjT0_S8_S8_T_");
asm(".globl fn_7101073060\n.set fn_7101073060, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7101072a80E7UnkIterIP17UnkObj_7101072a80EEEjT0_S8_S8_S8_T_");
asm(".globl fn_7101073150\n.set fn_7101073150, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_7101072a80E7UnkIterIP17UnkObj_7101072a80EEEjT0_S8_S8_S8_S8_T_");
asm(".globl fn_7101073280\n.set fn_7101073280, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7101072a80E7UnkIterIP17UnkObj_7101072a80EEEbT0_S8_T_");
