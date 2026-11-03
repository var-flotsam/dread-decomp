// CCharClassSoundEvent unit (tools/tu_map.py unit 2297): a constructor: the event base constructor
// (fn_7100cdc2b8), the vtable, eight file path ids built from "", an empty string id (its constructor
// inlined), the defaults (1, 500, 3000 and 1, then -1; -1 twice after two cleared words), string ids
// built from "DEFAULT" and "N", and a cleared word. As CCharClassLoopSoundEvent's
// (src/game/CCharClassLoopSoundEvent/CCharClassLoopSoundEvent_Ctor_b46.cpp). Names and the layout are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundEvent_b49 {
    void** vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId field_0x10[8];
    CStrId field_0x50;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    bool field_0x80;
    bool field_0x81;
    CStrId field_0x88;
    CStrId field_0x90;
    u32 field_0x98;
};
static_assert(__builtin_offsetof(UnkSoundEvent_b49, field_0x98) == 0x98, "layout");

void fn_7100cdc2b8(void* self);  // the base constructor
extern void* g_unk_7101aa63e0[];

void fn_7100cde6ac(UnkSoundEvent_b49* self) {
    fn_7100cdc2b8(self);
    self->vtable = &g_unk_7101aa63e0[2];
    for (CFilePathStrId& id : self->field_0x10)
        new (&id) CFilePathStrId("", true);
    self->field_0x50.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x50.data = &g_unk_7101d47108->empty;
    self->field_0x58 = 1.0f;
    self->field_0x5c = 500.0f;
    self->field_0x60 = 3000.0f;
    self->field_0x64 = 1.0f;
    self->field_0x68 = -1.0f;
    self->field_0x6c = false;
    self->field_0x70 = 0;
    self->field_0x74 = 0;
    self->field_0x78 = -1.0f;
    self->field_0x7c = -1.0f;
    self->field_0x80 = false;
    self->field_0x81 = false;
    new (&self->field_0x88) CStrId("DEFAULT", true);
    new (&self->field_0x90) CStrId("N", true);
    self->field_0x98 = 0;
}
