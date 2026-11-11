// CCharClassSetMaterialPropertyEvent's default constructor (the name from reflection): on the event
// base (fn_7100067ea4), an empty string id at +0x10, then words 0 and 1 and two cleared words.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CCharClassSetMaterialPropertyEvent.h"
#include "types.h"

using base::global::CStrId;

struct UnkSetMaterialPropertyEvent {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id;  // 0x10
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
    u64 field_0x28;
};

void fn_7100cd49ac(UnkSetMaterialPropertyEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa6108 + 0x10;
    new (&self->id) CStrId("", true);
    self->field_0x18 = 0;
    self->field_0x1c = 1;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
}
