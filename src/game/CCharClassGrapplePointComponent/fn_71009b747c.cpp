// A constructor in the CCharClassGrapplePointComponent unit, as the behaviour tree nodes' (for example
// src/game/behaviortree/CSetClockwiseDirBlcValueAction/fn_7100374984.cpp): the base constructor
// fn_71009bbbd8, the vtable, an empty string id at +0x68, 30 at +0x1c, then the post-construction
// virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkGrapplePointDef {
    void** vtable;
    u8 pad_0x8[0x14];
    u32 field_0x1c;
    u8 pad_0x20[0x48];
    UnkStrId field_0x68;
};

void fn_71009bbbd8(UnkGrapplePointDef* self);
extern void* g_unk_7101a880e8[];

void fn_71009b747c(UnkGrapplePointDef* self) {
    fn_71009bbbd8(self);
    self->vtable = &g_unk_7101a880e8[2];
    new (&self->field_0x68) UnkStrId("", true);
    self->field_0x1c = 30;
    reinterpret_cast<void (*)(UnkGrapplePointDef*)>(self->vtable[3])(self);
}
