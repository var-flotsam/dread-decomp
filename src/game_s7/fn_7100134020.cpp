// libc++ std::sort helpers instantiated for a container of 0x160-byte entries (through its own
// iterator class), sorted by the second string id's text with strcmp. An entry holds three
// pooled string ids and a string with 0x100 bytes of its own storage; its move assignment
// (fn_7100133054) and destructor (fn_7100133be4) are out of line. The comparator is internal,
// so the helpers take no comparator argument. Names and layouts are placeholders.
//
// None match yet. The original reuses the second id's pointer, read for the comparison, when a
// swap moves that entry; ours reads it again. That needs the alias analysis to know the swap's
// temporary isn't the entry, which it does only for pointer arguments (our iterators arrive as
// integers), and sorting raw pointers instead pairs stores the original keeps apart. Same
// behaviour.
#include <algorithm>
#include <cstring>
#include "../game_aca57c/UnkBufferString.h"
#include "UnkIter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrData_7100134020 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

// A pooled string id: moving it leaves the pool's empty string behind, and reading it bumps the
// data's use count.
struct UnkStr_7100134020 {
    UnkStrData_7100134020* data;

    UnkStr_7100134020(UnkStr_7100134020&& o) : data(o.data) {
        o.data = (UnkStrData_7100134020*)&g_unk_7101d47108->empty;
    }
    const char* CStr() const {
        ++data->field_0x44;
        return data->str ? data->str : "";
    }
};

extern void* g_unk_7101a194b8[];
using UnkString100_7100134020 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkEntry_7100134020 {
    UnkStr_7100134020 field_0x0;
    UnkStr_7100134020 name;
    UnkStr_7100134020 field_0x10;
    UnkString100_7100134020 field_0x18;

    UnkEntry_7100134020(UnkEntry_7100134020&&) = default;
    UnkEntry_7100134020& operator=(UnkEntry_7100134020&& o);  // fn_7100133054
    ~UnkEntry_7100134020();                                    // fn_7100133be4
};

namespace {

struct UnkCmp_7100134020 {
    bool operator()(const UnkEntry_7100134020& a, const UnkEntry_7100134020& b) const {
        return strcmp(a.name.CStr(), b.name.CStr()) < 0;
    }
};

}  // namespace

using P = UnkIter<UnkEntry_7100134020>;
using C = UnkCmp_7100134020;

// Not in the game: a direct call, so that the internal instantiations are emitted and their
// unused comparator arguments dropped, as in the game.
void unk_anchor_7100134020(P first, P last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments). The declarations below only carry the annotations.
asm(".globl fn_7100134020\n.set fn_7100134020, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_7100134020E7UnkIterI19UnkEntry_7100134020EEEvT0_S7_T_");
asm(".globl fn_7100134a60\n.set fn_7100134a60, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_7100134020E7UnkIterI19UnkEntry_7100134020EEEjT0_S7_S7_T_");
asm(".globl fn_7100134eb0\n.set fn_7100134eb0, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_7100134020E7UnkIterI19UnkEntry_7100134020EEEjT0_S7_S7_S7_T_");
asm(".globl fn_71001351b0\n.set fn_71001351b0, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_7100134020E7UnkIterI19UnkEntry_7100134020EEEjT0_S7_S7_S7_S7_T_");
asm(".globl fn_7100135580\n.set fn_7100135580, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_7100134020E7UnkIterI19UnkEntry_7100134020EEEbT0_S7_T_");
extern "C" {
// NON_MATCHING(minor): a swap reads the compared id's pointer again, see the note at the top
// of the file. Same behaviour.
void fn_7100134020()
    ;
// NON_MATCHING(minor): a swap reads the compared id's pointer again, see the note at the top
// of the file. Same behaviour.
void fn_7100134a60()
    ;
// NON_MATCHING(minor): a swap reads the compared id's pointer again, see the note at the top
// of the file. Same behaviour.
void fn_7100134eb0()
    ;
// NON_MATCHING(minor): a swap reads the compared id's pointer again, see the note at the top
// of the file. Same behaviour.
void fn_71001351b0()
    ;
// NON_MATCHING(minor): a swap reads the compared id's pointer again, see the note at the top
// of the file. Same behaviour.
void fn_7100135580()
    ;
}
