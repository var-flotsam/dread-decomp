// Unit 2200 (hint CCutSceneDef): the constructor of the 0x40-byte take record: empty string ids and an empty
// path id; the plain values are left unset. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkElem40_b42a3 {
    CStrId field_0x0;
    u32 field_0x8;
    CFilePathStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    u64 field_0x30;
    u32 field_0x38;
};

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100c9115c(UnkElem40_b42a3* self) {
    MakeEmpty(&self->field_0x0);
    new (&self->field_0x10) CFilePathStrId();
    MakeEmpty(&self->field_0x18);
    MakeEmpty(&self->field_0x20);
    MakeEmpty(&self->field_0x28);
}
