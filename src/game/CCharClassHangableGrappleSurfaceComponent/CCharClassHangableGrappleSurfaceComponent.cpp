// CCharClassHangableGrappleSurfaceComponent: its constructor (fn_7100823fe4). The base constructor
// fn_7100816b28, the class vtable, the float 0.5 at +0x68, an empty string id at +0x70 and the
// float 100 at +0x78, then the object's own vf_0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassGrapplePointComponent.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a758d8[];

struct UnkHangableGrappleSurface {
    void** vtable;
    u8 pad_0x8[0x60];
    f32 field_0x68;
    u8 pad_0x6c[4];
    CStrId field_0x70;
    f32 field_0x78;
};

void fn_7100823fe4(UnkHangableGrappleSurface* self) {
    fn_7100816b28(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a758d8 + 2;
    self->field_0x68 = 0.5f;
    new (&self->field_0x70) CStrId(g_unk_7101d470e0);
    self->field_0x78 = 100.0f;
    reinterpret_cast<void (*)(UnkHangableGrappleSurface*)>(self->vtable[3])(self);
}
