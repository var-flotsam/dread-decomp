// CMovementComponent unit: finds the ground below the owner (virtual +0x198 from a point raised by half the
// owner's height) and moves the owner there, lifted by dt; through the owner's virtual +0xd0 when
// teleport is set, else by writing its position. Then runs the pending-flag callback (virtual +0x260).
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

typedef UnkVec3f UnkSnapVec_l0d33ec;

struct UnkSnapActor_l0d33ec;

struct UnkSnapActorVt_l0d33ec {
    u8 pad_0x0[0xd0];
    void (*move)(UnkSnapActor_l0d33ec*, const UnkSnapVec_l0d33ec*, u32);  // 0xd0
};

struct UnkSnapActor_l0d33ec {
    UnkSnapActorVt_l0d33ec* vt;
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x324];
    f32 half_a;  // 0x38c
    u8 pad_0x390[4];
    f32 half_b;  // 0x394
    u8 pad_0x398[0x18];
    u8 dirty;  // 0x3b0
};

struct UnkSnapMovement_l0d33ec;

struct UnkSnapMovementVt_l0d33ec {
    u8 pad_0x0[0x198];
    UnkSnapVec_l0d33ec (*find_ground)(UnkSnapMovement_l0d33ec*, f32, const UnkSnapVec_l0d33ec*, void*, void*);  // 0x198
    u8 pad_0x1a0[0xc0];
    void (*pending)(UnkSnapMovement_l0d33ec*);  // 0x260
};

struct UnkSnapMovement_l0d33ec {
    UnkSnapMovementVt_l0d33ec* vt;
    u8 pad_0x8[0x28];
    UnkSnapActor_l0d33ec* owner;  // 0x30
    u8 pad_0x38[0xc1];
    u8 pending_flag;  // 0xf9
};

void fn_71010d33ec(UnkSnapMovement_l0d33ec* self, f32 dt, bool teleport) {
    UnkSnapActor_l0d33ec* o = self->owner;
    UnkSnapVec_l0d33ec from(o->x + 0.0f, o->y + (o->half_b + o->half_a * 0.5f), o->z + 0.0f);
    UnkSnapVec_l0d33ec ground = self->vt->find_ground(self, 1000.0f, &from, nullptr, nullptr);
    o = self->owner;
    if (teleport) {
        UnkSnapVec_l0d33ec to(ground.x + 0.0f, ground.y + dt, ground.z + 0.0f);
        o->vt->move(o, &to, 1);
    } else {
        o->x = ground.x + 0.0f;
        o->y = ground.y + dt;
        o->dirty = 1;
        o->z = ground.z + 0.0f;
    }
    if (self->pending_flag) {
        self->pending_flag = 0;
        self->vt->pending(self);
    }
}
