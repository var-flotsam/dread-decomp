// Unit 1159 (no name hint). fn_71007452d0: an object's position (+0x2c) plus its offset (+0xac); when
// the mode at +0xd8 is not 1 and it has an owner (+0x10), scaled by the owner's scale (+0x28) and
// moved by the owner's world position. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkOwner_1159_l7452d0 {
    u8 pad_0x0[0x28];
    f32 scale;  // 0x28
};

struct UnkOffsetObj_1159_l7452d0 {
    u8 pad_0x0[0x10];
    UnkOwner_1159_l7452d0* owner;  // 0x10
    u8 pad_0x18[0x14];
    f32 pos[3];  // 0x2c
    u8 pad_0x38[0x74];
    f32 offset[3];  // 0xac
    u8 pad_0xb8[0x20];
    s32 mode;  // 0xd8
};

// Same function as the one src/game/UnkUnit1161/UnkUnit1161_b42.cpp defines.
struct UnkPoint_1161_b42;
struct UnkVec3_1161_b42;
UnkVec3f fn_7100745f80(const UnkOwner_1159_l7452d0* owner);

UnkVec3f fn_71007452d0(const UnkOffsetObj_1159_l7452d0* self) {
    if (self->mode == 1) {
        return UnkVec3f(self->pos[0] + self->offset[0], self->pos[1] + self->offset[1], self->pos[2] + self->offset[2]);
    }
    if (self->owner) {
        UnkVec3f base = fn_7100745f80(self->owner);
        f32 x = self->pos[0] + self->offset[0];
        f32 y = self->pos[1] + self->offset[1];
        f32 z = self->pos[2] + self->offset[2];
        f32 s = self->owner->scale;
        return UnkVec3f(base.x + x * s, y * s + base.y, z * s + base.z);
    }
    f32 z = self->pos[2] + self->offset[2];
    f32 y = self->pos[1] + self->offset[1];
    f32 x = self->pos[0] + self->offset[0];
    return UnkVec3f(x, y, z);
}
