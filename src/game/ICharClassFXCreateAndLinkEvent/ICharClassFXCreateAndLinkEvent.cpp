// A constructor in the ICharClassFXCreateAndLinkEvent unit: the base constructor fn_7100067ea4, the vtable,
// an empty string id at +0x10, an empty file path at +0x20, an empty id at +0x30, then a transform (zero
// position and angles, unit scale) at +0x38, an empty id at +0x60, a cleared flag at +0x68 and -1.0 at +0x6c.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStrId;

struct UnkFXCreateAndLinkEvent {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkStrId field_0x10;
    u32 field_0x18;
    CFilePathStrId field_0x20;
    u32 field_0x28;
    UnkStrId field_0x30;
    f32 position[3];  // 0x38
    f32 angle[3];     // 0x44
    f32 scale[3];     // 0x50
    UnkStrId field_0x60;
    bool field_0x68;
    f32 field_0x6c;
};

extern char g_unk_7101aa08c8[];

void fn_7100bebf70(UnkFXCreateAndLinkEvent* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa08c8 + 0x10);
    new (&self->field_0x10) UnkStrId("", true);
    self->field_0x18 = 0;
    new (&self->field_0x20) CFilePathStrId("", true);
    self->field_0x28 = 0;
    new (&self->field_0x30) UnkStrId("", true);
    self->position[0] = 0.0f;
    self->position[1] = 0.0f;
    self->position[2] = 0.0f;
    self->angle[0] = 0.0f;
    self->angle[1] = 0.0f;
    self->angle[2] = 0.0f;
    self->scale[0] = 1.0f;
    self->scale[1] = 1.0f;
    self->scale[2] = 1.0f;
    new (&self->field_0x60) UnkStrId("", true);
    self->field_0x68 = false;
    self->field_0x6c = -1.0f;
}
