// CCharacterMovement's unit: position of the owner with the xy offset of an attached object added.
// Names and layouts are placeholders.
#include "types.h"
#include "base/math/CVector.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCmResult {
    f32 x, y, z;
    UnkCmResult() {}
    UnkCmResult(const UnkCmResult& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkCmAttach {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 pad_0x8[0x158 - 0x8];
    struct UnkCmAnchor* field_0x158;
};

struct UnkCmAnchor {
    virtual void vf_0x0();
    UnkCmResult vf_0x68_get() { return reinterpret_cast<UnkCmResult (**)(UnkCmAnchor*)>(*reinterpret_cast<void***>(this))[0x68 / 8](this); }
};

struct UnkCmSink {
    virtual void vf_0x0();
    void vf_0x178_set(void* a, int b) { reinterpret_cast<void (**)(UnkCmSink*, void*, int)>(*reinterpret_cast<void***>(this))[0x178 / 8](this, a, b); }
};

struct UnkCmOwner {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    f32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    f32 field_0x64;
    u8 pad_0x68[0x2a0 - 0x68];
    UnkCmSink* field_0x2a0;
};

struct UnkCm {
    u8 pad_0x0[0x30];
    UnkCmOwner* field_0x30;
    u8 pad_0x38[0x3b8 - 0x38];
    UnkCmAttach* field_0x3b8;
    UnkCmAttach* field_0x3c0;
};

UnkCmResult fn_7100979a60(UnkCm* self) {
    CVector2D xy;
    {
        UnkCmOwner* o = self->field_0x30;
        f32 x = o->field_0x44;
        f32 y = o->field_0x54;
        f32 z = o->field_0x64;
        CVector3D pos = {x, y, z};
        fn_710007b430(&xy, &pos);
    }
    UnkCmAttach* a = self->field_0x3c0;
    if (a || (a = self->field_0x3b8)) {
        UnkCmSink* s = self->field_0x30->field_0x2a0;
        if (s)
            s->vf_0x178_set(self->field_0x3b8, 0);
        UnkCmAnchor* anc = a->field_0x158;
        if (anc) {
            UnkCmResult p3 = anc->vf_0x68_get();
            CVector2D off;
            fn_710007b430(&off, reinterpret_cast<const CVector3D*>(&p3));
            xy.x = off.x + xy.x;
            xy.y = off.y + xy.y;
        }
    }
    UnkCmResult r;
    r.z = self->field_0x30->field_0x64;
    r.x = xy.x;
    r.y = xy.y;
    return r;
}
