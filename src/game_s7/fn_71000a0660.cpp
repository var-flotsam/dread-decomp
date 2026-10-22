// libc++ std::sort helpers instantiated for a container of object pointers (through its own
// iterator class), sorted by name with strcmp. The comparator is internal, so the helpers take no
// comparator argument. Names and layouts are placeholders.
#include <algorithm>
#include <cstring>
#include "types.h"
#include "UnkIter.h"

struct UnkStrData_71000a0660 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

// A string handle: reading it bumps the data's use count.
struct UnkStr_71000a0660 {
    UnkStrData_71000a0660* data;

    const char* CStr() const {
        ++data->field_0x44;
        return data->str ? data->str : "";
    }
};

struct UnkObj_71000a0660 {
    u8 pad_0x0[0x8];
    UnkStr_71000a0660 name;
};

namespace {

struct UnkCmp_71000a0660 {
    bool operator()(const UnkObj_71000a0660* a, const UnkObj_71000a0660* b) const {
        return strcmp(a->name.CStr(), b->name.CStr()) < 0;
    }
};

}  // namespace

using P = UnkIter<UnkObj_71000a0660*>;
using C = UnkCmp_71000a0660;

// Not in the game: a direct call, so that the internal instantiations are emitted and their
// unused comparator arguments dropped, as in the game.
void unk_anchor_71000a0660(P first, P last) {
    std::sort(first, last, C());
}

// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_71000a0660\n.set fn_71000a0660, _ZNSt3__16__sortIRN12_GLOBAL__N_117UnkCmp_71000a0660E7UnkIterIP17UnkObj_71000a0660EEEvT0_S8_T_");
asm(".globl fn_71000a0d50\n.set fn_71000a0d50, _ZNSt3__17__sort3IRN12_GLOBAL__N_117UnkCmp_71000a0660E7UnkIterIP17UnkObj_71000a0660EEEjT0_S8_S8_T_");
asm(".globl fn_71000a0f40\n.set fn_71000a0f40, _ZNSt3__17__sort4IRN12_GLOBAL__N_117UnkCmp_71000a0660E7UnkIterIP17UnkObj_71000a0660EEEjT0_S8_S8_S8_T_");
asm(".globl fn_71000a10c0\n.set fn_71000a10c0, _ZNSt3__17__sort5IRN12_GLOBAL__N_117UnkCmp_71000a0660E7UnkIterIP17UnkObj_71000a0660EEEjT0_S8_S8_S8_S8_T_");
asm(".globl fn_71000a12c0\n.set fn_71000a12c0, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_117UnkCmp_71000a0660E7UnkIterIP17UnkObj_71000a0660EEEbT0_S8_T_");
