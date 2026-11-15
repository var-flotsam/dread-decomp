// Unit 811 (CEmmyAIComponent hint): a point (w = 1) scaled by a value of the object at +0x29ee0; the
// base point depends on the AI state (a table entry 1 or 2, else zero). Names and layouts are placeholders.
#include "game/UnkTU811.h"
#include "types.h"

struct UnkScaleSource_551f0c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual struct UnkScaleValue_551f0c* vf_0xd8();
};

struct UnkScaleValue_551f0c {
    u8 pad_0x0[0x34];
    f32 scale;
};

struct UnkPoint4_551f0c {
    f32 x, y, z, w;
    UnkPoint4_551f0c(f32 x_, f32 y_, f32 z_) {
        w = 1.0f;
        x = x_;
        y = y_;
        z = z_;
    }
    UnkPoint4_551f0c(const UnkPoint4_551f0c& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkEmmyAI_551f0c {
    u8 pad_0x0[0x29ee0];
    UnkScaleSource_551f0c* source;
};

struct UnkVec3_551f0c {
    f32 x, y, z;
};

const UnkVec3_551f0c* fn_7100572110(u32 index);

UnkPoint4_551f0c fn_7100551f0c(UnkEmmyAI_551f0c* self) {
    f32 x = 0.0f, y = 0.0f, z = 0.0f;
    if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self))) {
        const UnkVec3_551f0c* p = fn_7100572110(1);
        x = p->x; y = p->y; z = p->z;
    } else if (fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self)) ||
               fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)) ||
               fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
        const UnkVec3_551f0c* p = fn_7100572110(2);
        x = p->x; y = p->y; z = p->z;
    }
    f32 s = self->source->vf_0xd8()->scale;
    return UnkPoint4_551f0c(x * s, y * s, z * s);
}
