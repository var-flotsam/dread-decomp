// CTrainUsableComponent's unit (tools/tu_map.py unit 1604): the constructor. On the usable base: the
// vtable, four empty string ids (one apart from the other three) and two flags. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CTrainUsableComponent.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a936e0[];

struct UnkTrainUsable_la9948c {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    UnkStringData* id;  // 0x540
    u8 pad_0x548[8];
    UnkStringData* ids[3];  // 0x550
    bool field_0x568;
    bool field_0x569;
};
static_assert(__builtin_offsetof(UnkTrainUsable_la9948c, field_0x568) == 0x568, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100a9948c(void* object) {
    UnkTrainUsable_la9948c* self = static_cast<UnkTrainUsable_la9948c*>(object);
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a936e0 + 2;
    MakeEmpty(&self->id);
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->field_0x568 = false;
    self->field_0x569 = true;
}
