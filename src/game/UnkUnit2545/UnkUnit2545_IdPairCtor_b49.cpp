// Unit 2545 (no reflection hint): a constructor: two cleared words, then two empty string ids (+0x10,
// +0x18), the id constructor inlined. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit2545IdPair {
    u64 field_0x0;
    u64 field_0x8;
    CStrId id_0x10;
    CStrId id_0x18;
};

// An empty id: the string id constructor for "" (the length folded) inlined.
__attribute__((always_inline)) static inline void InitEmptyId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100dd26a0(UnkUnit2545IdPair* self) {
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    InitEmptyId(&self->id_0x10);
    InitEmptyId(&self->id_0x18);
}
