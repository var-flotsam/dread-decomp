// CCharClassLoopSoundEvent unit (tools/tu_map.py unit 2262): a constructor: the event base
// constructor (fn_7100cdc2b8), the vtable, a file path id built from " ", a copy of the empty
// string id, the defaults (1, 500, 3000 and 1, two flags set, -1 twice) and a string id built
// from "N". Names and the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkLoopSoundEvent_b46a3 {
    void** vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId field_0x10;
    CStrId field_0x18;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    bool field_0x31;
    u8 unk_0x32[2];
    u32 field_0x34;
    u32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    bool field_0x44;
    CStrId field_0x48;
};

void fn_7100cdc2b8(void* self);  // the base constructor
extern void* g_unk_7101aa5d48[];

void fn_7100cc7b60(UnkLoopSoundEvent_b46a3* self) {
    fn_7100cdc2b8(self);
    self->vtable = &g_unk_7101aa5d48[2];
    new (&self->field_0x10) CFilePathStrId(" ", true);
    new (&self->field_0x18) CStrId(g_unk_7101d470e0);
    self->field_0x20 = 1.0f;
    self->field_0x24 = 500.0f;
    self->field_0x28 = 3000.0f;
    self->field_0x2c = 1.0f;
    self->field_0x30 = true;
    self->field_0x31 = true;
    self->field_0x34 = 0;
    self->field_0x38 = 0;
    self->field_0x3c = -1.0f;
    self->field_0x40 = -1.0f;
    self->field_0x44 = false;
    new (&self->field_0x48) CStrId("N", true);
}
