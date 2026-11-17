// CElevatorUsableComponent's unit (tools/tu_map.py unit 1591): the constructor. On the usable base: the
// vtable, four empty string ids (one before a flag, three after it) and the flag, set. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a91580[];

struct UnkElevatorUsable_la819ac {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    UnkStringData* id;  // 0x540
    bool field_0x548;
    u8 pad_0x549[0xf];
    UnkStringData* ids[3];  // 0x558
};
static_assert(__builtin_offsetof(UnkElevatorUsable_la819ac, ids) == 0x558, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100a819ac(void* object) {
    UnkElevatorUsable_la819ac* self = static_cast<UnkElevatorUsable_la819ac*>(object);
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a91580 + 2;
    MakeEmpty(&self->id);
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->field_0x548 = true;
}
