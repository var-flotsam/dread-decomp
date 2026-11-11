// CEnemyPreset's unit (tools/tu_map.py unit 819): a constructor (fn_7100578b90). The base
// constructor (fn_7101043904), the vtable 0x7101a49360, three empty ids copied from the global
// empty id around a -1.0f, a cleared flag, then the object's own vtable slot 3 as a tail call.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a49360[];

struct UnkEnemyPresetEntry_A3_b66 {
    void** vtable;
    u64 field_0x8;
    CStrId field_0x10;
    f32 field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    bool field_0x30;
};

void fn_7100578b90(UnkEnemyPresetEntry_A3_b66* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a49360) + 2;
    new (&self->field_0x10) CStrId(g_unk_7101d470e0);
    self->field_0x18 = -1.0f;
    new (&self->field_0x20) CStrId(g_unk_7101d470e0);
    new (&self->field_0x28) CStrId(g_unk_7101d470e0);
    self->field_0x30 = false;
    reinterpret_cast<void (*)(UnkEnemyPresetEntry_A3_b66*)>(self->vtable[3])(self);
}
