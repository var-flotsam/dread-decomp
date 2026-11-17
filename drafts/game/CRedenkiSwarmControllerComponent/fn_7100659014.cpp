// CRedenkiSwarmControllerComponent's unit: moves a swarm member to its point on the area's circle around
// the parent, turns its heading (0 to 255 around the circle) towards the way it moved when it moved more
// than 0.1 on any axis, and takes the controller's speed. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU1161.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
UnkVec3Result fn_71007455dc(UnkStruct_71007455dc* self);
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);
struct UnkTarget;
void fn_71007456fc(UnkTarget* target, u8 value);

struct UnkSwarmArea_l659014 {
    u8 pad_0x0[0x44];
    f32 radius;  // 0x44
};

struct UnkSwarmMember_l659014 {
    u8 pad_0x0[0x10];
    UnkSwarmArea_l659014* area;  // 0x10
    u8 pad_0x18[0x20];
    f32 direction[3];  // 0x38
    f32 field_0x44;
    u8 pad_0x48[0x118];
    f32 speed;  // 0x160
    u8 pad_0x164[4];
    f32 heading;  // 0x168
};
static_assert(__builtin_offsetof(UnkSwarmMember_l659014, heading) == 0x168, "member layout");

struct UnkSwarmController_l659014;

struct UnkSwarmControllerVtable_l659014 {
    void* pad_0x0[0x208 / 8];
    f32 (*GetSpeed)(UnkSwarmController_l659014* self);  // 0x208
};

struct UnkSwarmController_l659014 {
    UnkSwarmControllerVtable_l659014* vtable;
};

static inline bool NearZero_l659014(f32 v) { return v - 0.1f <= 0.0f && v + 0.1f >= 0.0f; }

static inline UnkVec3Result Normalized_l659014(f32 x, f32 y, f32 z) {
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
    return UnkVec3Result(x * inv, y * inv, z * inv);
}

void fn_7100659014(UnkSwarmController_l659014* self, UnkSwarmMember_l659014* member) {
    UnkSwarmArea_l659014* area = member->area;
    f32 radius = area->radius;
    UnkVec3Result center = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(area));
    f32 scale = member->field_0x44;
    UnkVec3Result pos(radius * member->direction[0], radius * member->direction[1],
                      radius * member->direction[2]);
    pos.x *= scale;
    pos.y *= scale;
    pos.z *= scale;
    pos.x += center.x;
    pos.y += center.y;
    pos.z += center.z;

    f32 heading = member->heading;
    UnkVec3Result current = fn_71007455dc(reinterpret_cast<UnkStruct_71007455dc*>(member));
    f32 dx = pos.x - current.x;
    f32 dy = pos.y - current.y;
    f32 dz = pos.z - current.z;
    if (!(NearZero_l659014(dx) && NearZero_l659014(dy) && NearZero_l659014(dz))) {
        UnkVec3Result way = Normalized_l659014(dx, dy, dz);
        UnkVec3Result right = Normalized_l659014(1.0f, 0.0f, 0.0f);
        f32 cosine = way.x * right.x + way.y * right.y + way.z * right.z;
        cosine = !(cosine >= 1.0f) ? cosine : 1.0f;
        cosine = !(cosine < -1.0f) ? cosine : -1.0f;
        f32 angle = acosf(cosine);
        if (dy < 0.0f)
            angle = 6.2831855f - angle;
        heading = angle * 255.0f / 6.2831855f;
    }

    fn_710074566c(reinterpret_cast<UnkStruct_71007455dc*>(member), &pos.x);
    member->heading = heading;
    member->speed = self->vtable->GetSpeed(self);
    fn_71007456fc(reinterpret_cast<UnkTarget*>(member), heading);
}
