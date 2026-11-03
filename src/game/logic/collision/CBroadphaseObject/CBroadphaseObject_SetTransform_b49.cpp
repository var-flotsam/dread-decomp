// game::logic::collision::CBroadphaseObject's unit: keeps the current 16-value transform (+0x6c) as the
// previous one (+0xac), takes the new one and lets the object update (vf_0x38). Names and layouts are
// placeholders.
#include "types.h"

struct UnkBroadphaseXformObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    u8 pad_0x8[0x64];
    f32 xform[16];       // 0x6c
    f32 prev_xform[16];  // 0xac
};

void fn_7100d35f14(UnkBroadphaseXformObject* self, const f32* xform) {
    for (u32 i = 0; i < 16; i++)
        self->prev_xform[i] = self->xform[i];
    for (u32 i = 0; i < 16; i++)
        self->xform[i] = xform[i];
    self->vf_0x38();
}
