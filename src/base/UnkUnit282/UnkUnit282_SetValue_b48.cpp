// Unit 282 (tools/tu_map.py, the widget of src/base/UnkTaggedObjectSubCtors_b40.cpp): sets a
// three-component value through its own virtual slot 0x30, forwards it to the element at +0x108
// (fn_71001b7bb8) and marks the widget dirty (+0xfb). Names and layouts are placeholders.
#include "base/math/CVector.h"

using base::math::CVector3D;

struct UnkUnit282Element;
void fn_71001b7bb8(UnkUnit282Element* element, void* key, f32 x, f32 y, f32 z);

struct UnkUnit282Widget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(void* key, CVector3D v);

    u8 pad_0x8[0xf3];
    bool dirty;                   // 0xfb
    u8 pad_0xfc[0xc];
    UnkUnit282Element* element;   // 0x108
};

// The value comes first in the signature: it sets the original's register assignment (the
// registers are the same: self and key in x0/x1, the value in s0-s2).
void fn_7100205f98(UnkUnit282Widget* self, CVector3D v, void* key) {
    self->vf_0x30(key, v);
    fn_71001b7bb8(self->element, key, v.x, v.y, v.z);
    self->dirty = true;
}
