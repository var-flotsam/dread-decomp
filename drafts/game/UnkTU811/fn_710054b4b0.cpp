#include "game/UnkXformNode.h"
#include "types.h"

struct UnkScaleVtbl_b0193 {
    void* unk_0x0[27];
    f32* (*vf_0xd8)(void* self);
};
struct UnkScaleObj_b0193 {
    UnkScaleVtbl_b0193* vtbl;
};
struct UnkVecAI_b0193 {
    u8 unk_0x0[0x29ee0];
    UnkScaleObj_b0193* field_0x29ee0;
    u8 unk_0x29ee8[0x29f04 - 0x29ee8];
    f32 field_0x29f04;
};

struct UnkVec4_b0193 {
    f32 x, y, z, w;
    UnkVec4_b0193(f32 x_, f32 y_, f32 z_, f32 w_) {
        w = w_;
        x = x_;
        y = y_;
        z = z_;
    }
    UnkVec4_b0193(const UnkVec4_b0193& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

bool fn_710054ad2c(UnkVecAI_b0193*);
bool fn_710054aecc(UnkVecAI_b0193*);
bool fn_7100543b30(UnkVecAI_b0193*);
bool fn_71005436e8(UnkVecAI_b0193*);
const UnkVec3f* fn_7100572110(u32 index);
extern f32 g_unk_7101a482c0;
extern f32 g_unk_7101a482c4;

UnkVec4_b0193 fn_710054b4b0(UnkVecAI_b0193* self) {
    UnkVec3f v;
    if (fn_710054ad2c(self) || fn_710054aecc(self)) {
        v = *fn_7100572110(0);
    } else if (fn_7100543b30(self)) {
        v = *fn_7100572110(1);
    } else if (fn_71005436e8(self)) {
        v = *fn_7100572110(2);
        f32 t = self->field_0x29f04;
        f32 lo = g_unk_7101a482c0;
        if (t > lo) {
            f32 range = g_unk_7101a482c4;
            if (t <= lo + range) {
                f32 f = 1.0f - (t - lo) / range;
                v.x = f * v.x;
                v.y = f * v.y;
                v.z = f * v.z;
            }
        }
    } else {
        v = UnkVec3f(0.0f, 0.0f, 0.0f);
    }
    f32 scale = self->field_0x29ee0->vtbl->vf_0xd8(self->field_0x29ee0)[0x34 / 4];
    f32 ox = scale * v.x;
    f32 oy = scale * v.y;
    f32 oz = scale * v.z;
    return UnkVec4_b0193(ox, oy, oz, 1.0f);
}
