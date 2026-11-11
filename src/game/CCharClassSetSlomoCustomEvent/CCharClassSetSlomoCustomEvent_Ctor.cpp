// CCharClassSetSlomoCustomEvent's default constructor (the name from reflection): on the event base
// (fn_7100067ea4), two factors of 1 at +0xc and +0x10, then an empty string id at +0x18. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa6290[];

struct UnkSetSlomoCustomEvent {
    void* vtable;
    u8 unk_0x8[0x4];
    f32 field_0xc;
    f32 field_0x10;
    u8 unk_0x14[0x4];
    CStrId id;  // 0x18
};

void fn_7100cd8df4(UnkSetSlomoCustomEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa6290 + 0x10;
    self->field_0xc = 1.0f;
    self->field_0x10 = 1.0f;
    new (&self->id) CStrId("", true);
}
