// Unit 392 (tools/tu_map.py): the destructor and the deleting destructor of a class holding two growable
// arrays (base/UnkVector.h): bytes at +0x40, and at +0x18 0x40-byte records destroyed by fn_71002b67e8
// (src/game/UnkUnit377/UnkRefVectorHolder_Dtor_b47.cpp). Names are placeholders.
#include "base/UnkVector.h"

struct UnkRefVectorHolder_b47;
void fn_71002b67e8(UnkRefVectorHolder_b47* self);

// A 0x40-byte element whose destructor is out of line (fn_71002b67e8: frees its array, releases its
// counted object).
struct UnkHolderElem_b47 {
    u8 data[0x40];
    ~UnkHolderElem_b47() { fn_71002b67e8(reinterpret_cast<UnkRefVectorHolder_b47*>(this)); }
};

extern void* g_unk_7101a214f8[];  // the class's vtable

struct UnkHolderOwner_b47 {
    void** vtable;
    u8 pad_0x8[0x10];
    base::UnkVector<UnkHolderElem_b47> holders;  // 0x18
    base::UnkVector<u8> bytes;                   // 0x40

    ~UnkHolderOwner_b47() {
        vtable = &g_unk_7101a214f8[2];
        bytes.Free();
        holders.Free();
    }
};

void fn_71002c5394(UnkHolderOwner_b47* self) { self->~UnkHolderOwner_b47(); }
void fn_71002c5450(UnkHolderOwner_b47* self) {
    self->~UnkHolderOwner_b47();
    __builtin_operator_delete(self);
}
