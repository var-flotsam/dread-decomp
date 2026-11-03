// CCameraFX_Shake's constructor (shaped as fn_7100823fe4,
// src/game/CCharClassHangableGrappleSurfaceComponent/CCharClassHangableGrappleSurfaceComponent.cpp):
// the base constructor fn_71010f0e04, the class vtable, four cleared words, an
// empty string id, a cleared word and flag, then the object's own vf_0x18. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_71010f0e04(char* self);
extern void* g_unk_7101cb1d58[];

struct UnkCameraFXShake {
    void** vtable;
    u8 pad_0x8[0x2c];
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u8 pad_0x44[4];
    CStrId field_0x48;
    u32 field_0x50;
    bool field_0x54;
};

void fn_71010f4050(UnkCameraFXShake* self) {
    fn_71010f0e04(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101cb1d58 + 2;
    self->field_0x34 = 0;
    self->field_0x38 = 0;
    self->field_0x3c = 0;
    self->field_0x40 = 0;
    new (&self->field_0x48) CStrId(g_unk_7101d470e0);
    self->field_0x50 = 0;
    self->field_0x54 = false;
    reinterpret_cast<void (*)(UnkCameraFXShake*)>(self->vtable[3])(self);
}
