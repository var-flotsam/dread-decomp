// Constructor of a pair of string ids, both the empty string (creating the pool first when there is none).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkIdPair {
    UnkStringData* field_0x0;
    UnkStringData* field_0x8;
};

void fn_71002dacc0(UnkIdPair* self) {
    self->field_0x0 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x0 = &g_unk_7101d47108->empty;
    self->field_0x8 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x8 = &g_unk_7101d47108->empty;
}
