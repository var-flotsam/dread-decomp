// CDoorGrapplePointComponent (tools/tu_map.py; the name from reflection): the grapple position.
// With an attached object (+0x40, its +0x10), the position of that object's actor (+0x30, the
// translation at +0x44/+0x54/+0x64) goes through fn_710082971c; otherwise it is the owner's
// (+0x30) position. Names and layout are placeholders.
#include "game/UnkActor.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable (as in
// src/game_lo/fn_71007455dc.cpp).
struct UnkVec3Result_b44a2 {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result_b44a2(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result_b44a2(const UnkVec3Result_b44a2& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkGrappleAttached_b44a2 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

struct UnkGrappleLink_b44a2 {
    u8 pad_0x0[0x10];
    UnkGrappleAttached_b44a2* attached;
};

struct UnkDoorGrapplePoint_b44a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x8];
    UnkGrappleLink_b44a2* link;
};

UnkVec3Result_b44a2 fn_710082971c(UnkDoorGrapplePoint_b44a2* self, const UnkVec3Result_b44a2* pos);

UnkVec3Result_b44a2 fn_71008296b0(UnkDoorGrapplePoint_b44a2* self) {
    if (self->link && self->link->attached) {
        UnkActor* actor = self->link->attached->actor;
        UnkVec3Result_b44a2 pos(actor->field_0x44, actor->field_0x54, actor->field_0x64);
        return fn_710082971c(self, &pos);
    }
    UnkActor* owner = self->owner;
    return UnkVec3Result_b44a2(owner->field_0x44, owner->field_0x54, owner->field_0x64);
}
