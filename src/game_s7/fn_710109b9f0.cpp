// libc++ std::sort helpers instantiated for a list of 16-byte entries (through its own iterator
// class), ordered by the string the first field points to (fn_710007f320). The game's __sort4 was
// folded into another copy. Names and layouts are placeholders.
#include <algorithm>
#include "UnkIter.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

bool fn_710007f320(const CRntString* a, const CRntString* b);

struct UnkEntry_710109b9f0 {
    const CRntString* key;
    u64 field_0x8;
};

namespace {

struct UnkCmp_710109b9f0 {
    bool operator()(const UnkEntry_710109b9f0& a, const UnkEntry_710109b9f0& b) const {
        return fn_710007f320(a.key, b.key);
    }
};

}  // namespace

using P = UnkIter<UnkEntry_710109b9f0>;
using C = UnkCmp_710109b9f0;

// Not in the game: a direct call, so that the internal instantiations are emitted and their unused
// comparator arguments dropped, as in the game.
void unk_anchor_710109b9f0(P first, P last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_710109b9f0\n.set fn_710109b9f0, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_710109b9f0E7UnkIterI19UnkEntry_710109b9f0EEEvT0_S7_T_");
asm(".globl fn_710109be90\n.set fn_710109be90, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_710109b9f0E7UnkIterI19UnkEntry_710109b9f0EEEjT0_S7_S7_T_");
asm(".globl fn_710109bfb0\n.set fn_710109bfb0, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_710109b9f0E7UnkIterI19UnkEntry_710109b9f0EEEjT0_S7_S7_S7_S7_T_");
asm(".globl fn_710109c170\n.set fn_710109c170, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_710109b9f0E7UnkIterI19UnkEntry_710109b9f0EEEbT0_S7_T_");
