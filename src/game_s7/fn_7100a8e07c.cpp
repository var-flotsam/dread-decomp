// Two more copies of fn_71002dacc0 (src/game_s2/): the constructor of a pair of string ids, both
// the empty string (creating the pool first when there is none).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkIdPair_7100a8e07c {
    UnkStringData* field_0x0;
    UnkStringData* field_0x8;
};

static inline void Construct_7100a8e07c(UnkIdPair_7100a8e07c* self) {
    self->field_0x0 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x0 = &g_unk_7101d47108->empty;
    self->field_0x8 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x8 = &g_unk_7101d47108->empty;
}

void fn_7100a8e07c(UnkIdPair_7100a8e07c* self) {
    Construct_7100a8e07c(self);
}

void fn_7100ef1970(UnkIdPair_7100a8e07c* self) {
    Construct_7100a8e07c(self);
}
