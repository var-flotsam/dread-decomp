// A constructor in the CCameraFX_RandShake unit, as fn_71009b747c
// (src/game/CCharClassGrapplePointComponent/fn_71009b747c.cpp): the base constructor
// fn_71010f3d20, the vtable, an empty string id at +0x58, three zeroed words after it, then the post-construction
// virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkRandShakeDef {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkStrId field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
};

void fn_71010f3d20(UnkRandShakeDef* self);
extern void* g_unk_7101cb1c98[];

void fn_71010f2b68(UnkRandShakeDef* self) {
    fn_71010f3d20(self);
    self->vtable = &g_unk_7101cb1c98[2];
    new (&self->field_0x58) UnkStrId("", true);
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    reinterpret_cast<void (*)(UnkRandShakeDef*)>(self->vtable[3])(self);
}
