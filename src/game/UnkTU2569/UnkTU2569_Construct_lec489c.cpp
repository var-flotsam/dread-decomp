// Unit 2569: a record's constructor: its kind, two string ids copied from the arguments, and an empty
// array of 32-byte entries (flags 0xf). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The unit's entry array (src/game/UnkTU2569/UnkTU2569_Index_ovb0245.cpp).
template <bool kSwapped>
struct UnkIdEntryT_ovb0245;
using UnkIdVec_ovb0245 = base::global::CRntVector<UnkIdEntryT_ovb0245<true>>;

struct UnkIdRecord_lec489c {
    u32 kind;
    CStrId field_0x8;
    CStrId field_0x10;
    UnkIdVec_ovb0245 entries;  // 0x18
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100ec489c(UnkIdRecord_lec489c* self, u32 kind, const CStrId* a, const CStrId* b) {
    InitEmptyStrId(&self->field_0x8);
    InitEmptyStrId(&self->field_0x10);
    new (&self->entries) UnkIdVec_ovb0245(0xf);
    self->kind = kind;
    self->field_0x8 = *a;
    self->field_0x10 = *b;
}
