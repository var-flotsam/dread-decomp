// CCharClassSetCameraNearEvent's default constructor (the name from reflection): on the event base
// (fn_7100067ea4), a distance of 100 at +0xc, an empty string id at +0x10 and a cleared word at
// +0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa6048[];

struct UnkSetCameraNearEvent {
    void* vtable;
    u8 unk_0x8[0x4];
    f32 distance;  // 0xc
    CStrId id;  // 0x10
    u32 field_0x18;
};

void fn_7100cd1cd8(UnkSetCameraNearEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa6048 + 0x10;
    self->distance = 100.0f;
    new (&self->id) CStrId("", true);
    self->field_0x18 = 0;
}
