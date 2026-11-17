// Unit 1454: measures where the target stands relative to a position: the flat offset to it, its
// distance and direction, the facing projected on that direction and on its side, and which way to turn.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMove_b152;
f32 fn_71009e5114(UnkMove_b152* self);

// The tracked object: a position source with a virtual 0x188 that gives a position.
class UnkTarget_l9842e4 {
public:
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
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

struct UnkOwner_l9842e4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};

struct UnkActor_l9842e4 {
    u8 pad_0x0[0x30];
    UnkOwner_l9842e4* owner;  // 0x30
    u8 pad_0x38[0x1d8 - 0x38];
    UnkVec3f facing;  // 0x1d8
};

struct UnkInner_l9842e4 {
    u8 pad_0x0[0x38];
    UnkMove_b152* move;  // 0x38
};

struct UnkOuter_l9842e4 {
    u8 pad_0x0[0x8];
    UnkInner_l9842e4* inner;  // 0x8
};

struct UnkSelf_l9842e4 {
    u8 pad_0x0[0x8];
    UnkOuter_l9842e4* outer;  // 0x8
    u8 pad_0x10[0x3e4 - 0x10];
    f32 field_0x3e4;
};

UnkTarget_l9842e4* fn_710097f1c8(UnkSelf_l9842e4* self);
UnkActor_l9842e4* fn_710097f188(UnkSelf_l9842e4* self);

struct UnkAim_l9842e4 {
    UnkVec3f offset{0.0f, 0.0f, 0.0f};  // 0x0
    f32 distance = 0.0f;                // 0xc
    UnkVec3f dir{0.0f, 0.0f, 0.0f};     // 0x10
    UnkVec3f side{0.0f, 0.0f, 0.0f};    // 0x1c
    UnkVec3f facing{0.0f, 0.0f, 0.0f};  // 0x28
    f32 ahead = 0.0f;                   // 0x34
    f32 across = 0.0f;                  // 0x38
    UnkVec3f up{0.0f, 0.0f, 0.0f};      // 0x3c
    bool inFront = false;               // 0x48
    bool near = false;                  // 0x49
    f32 turn = 0.0f;                    // 0x4c
};
static_assert(sizeof(UnkAim_l9842e4) == 0x50, "aim");

static inline f32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

UnkAim_l9842e4 fn_71009842e4(UnkSelf_l9842e4* self, const UnkVec3f* from, f32 preferred) {
    UnkAim_l9842e4 aim;
    UnkTarget_l9842e4* target = fn_710097f1c8(self);
    UnkActor_l9842e4* actor = fn_710097f188(self);
    UnkOwner_l9842e4* owner = actor->owner;
    aim.facing = actor->facing;
    UnkVec3f pos = target->vf_0x188();
    f32 dx = pos.x - from->x;
    f32 dy = pos.y - from->y;
    aim.offset.z = 0.0f;
    aim.offset.y = dy;
    aim.offset.x = dx;
    if (self->field_0x3e4 != 0.0f) {
        aim.up = UnkVec3f(self->field_0x3e4, 0.0f, 0.0f);
    } else {
        const UnkMatrix44& m = fn_710002614c(&owner->xform);
        aim.up = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    }

    f32 lengthSq = aim.offset.x * aim.offset.x + aim.offset.y * aim.offset.y + aim.offset.z * aim.offset.z;
    aim.distance = 0.0f;
    if (!(lengthSq > 1e-5f))
        return aim;
    f32 length = sqrtf(lengthSq);
    f32 inv = 1.0f / length;
    aim.distance = length;
    f32 dirX = inv * aim.offset.x;
    f32 dirY = inv * aim.offset.y;
    f32 dirZ = inv * aim.offset.z;
    aim.ahead = aim.facing.x * dirX + aim.facing.y * dirY + aim.facing.z * dirZ;
    aim.dir.z = dirZ;
    aim.dir.x = dirX;
    aim.dir.y = dirY;
    // The side: the direction crossed with the up axis.
    f32 sideX = dirY * 1.0f - dirZ * 0.0f;
    f32 sideY = dirZ * 0.0f - dirX * 1.0f;
    f32 sideZ = dirX * 0.0f - dirY * 0.0f;
    aim.side.y = sideY;
    aim.side.z = sideZ;
    aim.side.x = sideX;
    aim.across = aim.facing.x * sideX + aim.facing.y * sideY + aim.facing.z * sideZ;
    aim.near = fabsf(length) <= 50.0f;
    aim.inFront = fabsf(aim.across) <= 350.0f;
    if (!aim.inFront) {
        aim.turn = Sign(aim.across);
        if (fn_710097f1c8(self) != nullptr && aim.distance > 1e-5f)
            fn_71009e5114(self->outer->inner->move);
    } else if (!aim.near) {
        aim.turn = Sign(aim.across);
    } else {
        aim.turn = Sign(preferred);
    }
    return aim;
}
