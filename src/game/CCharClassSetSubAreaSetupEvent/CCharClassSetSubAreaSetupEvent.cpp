// CCharClassSetSubAreaSetupEvent's default constructor (shaped as fn_71002536b0,
// src/game/CActorComponentDef/CActorComponentDef_Ctor.cpp): on the event base (fn_7100067ea4), two
// string ids copied from the empty id g_unk_7101d470e0, then a cleared flag. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern char g_unk_7101aa62c0[];

struct UnkSubAreaSetupEvent {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
    bool flag_0x20;
};

static inline void CopyEmptyId(CStrId* id, UnkStringData* data) {
    id->data = data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

void fn_7100cd99ec(UnkSubAreaSetupEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa62c0 + 0x10;
    UnkStringData* empty = g_unk_7101d470e0.data;
    CopyEmptyId(&self->id_0x10, empty);
    CopyEmptyId(&self->id_0x18, empty);
    self->flag_0x20 = false;
}
