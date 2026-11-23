// CTeleporterUsableComponent's unit (tools/tu_map.py unit 1600): a usable component's constructor: the
// usable base, the vtable, three empty string ids, a pair of ids, a listener and a callback set.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkIdPair_7100a8e07c.h"
#include "game/UnkTU1600.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;

struct UnkUsableCallbacks_la8de20 {
    void** vtable;
    CRntVector<void*> items;  // 0x8
    u32 field_0x30;
    u64 field_0x38;
};

struct UnkUsableListener_la8de20 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;

    UnkUsableListener_la8de20() : vtable(g_unk_7101a22d48 + 2), field_0x8(0), field_0x10(0), field_0x18(0) {}
};

struct UnkUsable_la8de20 {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    UnkStringData* ids[3];  // 0x540
    UnkIdPair_7100a8e07c pair;  // 0x558
    UnkUsableListener_la8de20 listener;  // 0x568
    u8 pad_0x588[0x8];
    UnkUsableCallbacks_la8de20 callbacks;  // 0x590
};
static_assert(__builtin_offsetof(UnkUsable_la8de20, callbacks) == 0x590, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100a8de20(UnkUsable_la8de20* self) {
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a92b00 + 2;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    fn_7100a8e07c(&self->pair);

    new (&self->listener) UnkUsableListener_la8de20;

    UnkUsableCallbacks_la8de20& callbacks = self->callbacks;
    callbacks.vtable = g_unk_7101a20998 + 2;
    new (&callbacks.items) CRntVector<void*>(15);
    callbacks.field_0x30 = 0;
    callbacks.vtable = g_unk_7101a33fa0 + 2;
    callbacks.field_0x38 = 0;
}
