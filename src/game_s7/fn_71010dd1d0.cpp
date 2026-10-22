// libc++ std::sort helpers instantiated for a list of entry pointers, ordered by depth (the float
// at +0x38): of two entries within 1e-5 of each other, one that isn't in a breakable tile group
// goes before one that is; any other pair compares equal. The game's __sort3 and __sort5
// (fn_71010dde80, fn_71010de570) are defined in src/game_hi/. Names and layouts are placeholders.
#include <algorithm>
#include "../../lib/libcxx/sort.h"
#include "types.h"

struct UnkClass;
namespace CBreakableTileGroup {
UnkClass* refl_GetClass();
}
bool fn_710009d50c(UnkClass*, UnkClass*);

struct UnkReflObj_71010dd1d0 {
    virtual UnkClass* vf_0x0();
};

struct UnkInner_71010dd1d0 {
    u8 pad_0x0[0x10];
    UnkReflObj_71010dd1d0* field_0x10;
};

struct UnkOwner_71010dd1d0 {
    u8 pad_0x0[0x10];
    UnkInner_71010dd1d0* field_0x10;
};

struct UnkEntry_71010dd1d0 {
    UnkOwner_71010dd1d0* field_0x0;
    u8 pad_0x8[0x30];
    f32 field_0x38;
};

static inline bool IsTileGroup_71010dd1d0(const UnkEntry_71010dd1d0* e) {
    if (UnkOwner_71010dd1d0* o = e->field_0x0)
        if (UnkInner_71010dd1d0* i = o->field_0x10)
            if (UnkReflObj_71010dd1d0* r = i->field_0x10)
                return fn_710009d50c(r->vf_0x0(), CBreakableTileGroup::refl_GetClass());
    return false;
}

static inline bool Near_71010dd1d0(const f32& x, const f32& y) {
    return x - 1e-5f <= y && x + 1e-5f >= y;
}

namespace {

struct UnkCmp_71010dd1d0 {
    // Declared inline: the hint raises the inlining threshold, which the comparator needs to be
    // inlined into every helper as in the game.
    inline bool operator()(const UnkEntry_71010dd1d0* a, const UnkEntry_71010dd1d0* b) const {
        if (Near_71010dd1d0(a->field_0x38, b->field_0x38))
            return !IsTileGroup_71010dd1d0(a) & IsTileGroup_71010dd1d0(b);
        return false;
    }
};

}  // namespace

using P = UnkEntry_71010dd1d0*;
using C = UnkCmp_71010dd1d0;

// Not in the game (its caller is fn_71010dbd54): a direct call, so that the internal
// instantiations are emitted and their unused comparator arguments dropped, as in the game.
void unk_anchor_71010dd1d0(P* first, P* last) {
    C c;
    libcxx_adapted::__sort<C&>(first, last, c);
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments). The declarations below only carry the annotations.
asm(".globl fn_71010dd1d0\n.set fn_71010dd1d0, _ZN14libcxx_adapted6__sortIRN12_GLOBAL__N_117UnkCmp_71010dd1d0EPP19UnkEntry_71010dd1d0EEvT0_S7_T_");
asm(".globl fn_71010de270\n.set fn_71010de270, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71010dd1d0EPP19UnkEntry_71010dd1d0EEjT0_S7_S7_S7_T_");
asm(".globl fn_71010de950\n.set fn_71010de950, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71010dd1d0EPP19UnkEntry_71010dd1d0EEbT0_S7_T_");
extern "C" {
// NON_MATCHING(minor): the adapted std::__sort; the original keeps the end pointer in x26 and
// the swap count in w19 where ours spills them, so other values move between registers and the
// stack. Same behaviour.
void fn_71010dd1d0()
    ;
// NON_MATCHING(minor): the insertion loop walks an index where the original walks a pointer, and
// the blocks are laid out in another order; same behaviour.
void fn_71010de950()
    ;
}
