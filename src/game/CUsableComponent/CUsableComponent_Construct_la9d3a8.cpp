// CUsableComponent's unit (tools/tu_map.py): the constructor of a tunable with six string ids (destroyed by
// fn_7100a9d710): the tunable base, its vtable, then each id empty. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a94638[];

struct UnkUsableTunable_la9d3a8 : UnkTunable {
    u8 pad_0x8[0x28];
    CStrId field_0x30;
    CStrId field_0x38;
    u64 field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
};
static_assert(__builtin_offsetof(UnkUsableTunable_la9d3a8, field_0x60) == 0x60, "UnkUsableTunable_la9d3a8 layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100a9d3a8(UnkUsableTunable_la9d3a8* self) {
    fn_7100136ecc(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a94638 + 0x10);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x50);
    MakeEmpty(&self->field_0x58);
    MakeEmpty(&self->field_0x60);
}
