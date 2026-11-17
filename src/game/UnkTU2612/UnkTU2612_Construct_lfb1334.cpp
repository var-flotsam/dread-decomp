// Unit 2612 of tools/tu_map.py (no name hint): the construct slot of a reflected array of nine string ids
// (0x48 bytes): each id starts empty, creating the pool first when there is none. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The nine ids.
struct UnkStrIds_lfb1334 {
    CStrId field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
};
static_assert(sizeof(UnkStrIds_lfb1334) == 0x48, "nine ids");

void fn_7100fb1334(UnkStrIds_lfb1334* self) {
    MakeEmpty(&self->field_0x0);
    MakeEmpty(&self->field_0x8);
    MakeEmpty(&self->field_0x10);
    MakeEmpty(&self->field_0x18);
    MakeEmpty(&self->field_0x20);
    MakeEmpty(&self->field_0x28);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x40);
}
