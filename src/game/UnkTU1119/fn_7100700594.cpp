#include "types.h"

extern "C" f32 sqrtf(f32);

struct UnkPlayerLike_b0197 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20(f32 value);
    virtual s32 vf_0x28();
    virtual UnkPlayerLike_b0197* vf_0x30(s32 index);
};

struct UnkAim_b0197 {
    f32 radius_0x0;
    u8 use_radius_0x4;
    u8 pad_0x5[0x1b];
    UnkPlayerLike_b0197* target_0x20;
    u8 flag_0x28;
    u8 pad_0x29[0x137];
    f32 a_x_0x160;
    f32 a_y_0x164;
    f32 b_x_0x168;
    f32 b_y_0x16c;
};

struct UnkAimOwner_b0197 {
    u8 pad_0x0[0x12cc0];
    UnkAim_b0197 aim_0x12cc0;
};

struct UnkVec2_b0197 {
    f32 x, y;
};

// The unit direction of the two floats at v, read from memory again after the length is taken.
static inline UnkVec2_b0197 Normalized(const f32* v) {
    f32 len = sqrtf(v[0] * v[0] + v[1] * v[1]);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        return {inv * v[0], inv * v[1]};
    }
    return {v[0], v[1]};
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7100700594(UnkAimOwner_b0197* self, f32 threshold) {
    UnkAim_b0197* aim = &self->aim_0x12cc0;
    if (!aim->flag_0x28)
        return false;
    UnkPlayerLike_b0197* target = aim->target_0x20;
    if (!target)
        return false;
    if (!target->vf_0x20(threshold))
        return false;
    f32 reach = aim->use_radius_0x4 ? aim->radius_0x0 * 0.2f : 0.0f;
    if (target->vf_0x30(0)->vf_0x28() != 2)
        return false;
    UnkVec2_b0197 b = Normalized(&aim->b_x_0x168);
    UnkVec2_b0197 a = Normalized(&aim->a_x_0x160);
    if (a.y * b.y + a.x * b.x < threshold)
        return aim->a_x_0x160 * aim->a_x_0x160 + aim->a_y_0x164 * aim->a_y_0x164 > reach * reach;
    return false;
}
