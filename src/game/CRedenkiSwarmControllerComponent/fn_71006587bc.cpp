// CRedenkiSwarmControllerComponent's unit: (re)starts a swarm member if it is flagged and accepted, picks
// its speed in the controller's range, and points it from its orbit position towards a random point within
// reach (at most 15) around it. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU1159.h"
#include "types.h"

extern "C" int rand();

struct UnkRedenkiController_b55;
struct UnkTarget;
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);

struct UnkSwarmArea_l6587bc {
    u8 pad_0x0[0x44];
    f32 radius;  // 0x44
};

struct UnkSwarmMember_l6587bc {
    u8 pad_0x0[0x10];
    UnkSwarmArea_l6587bc* area;  // 0x10
    u8 pad_0x18[0x20];
    f32 direction[3];  // 0x38
    f32 orbit_radius;  // 0x44
    f32 orbit_angle;   // 0x48
    u8 pad_0x4c[0x10c];
    f32 speed;         // 0x158
    f32 target_speed;  // 0x15c
    u8 pad_0x160[0x20];
    u8 field_0x180;
};
static_assert(__builtin_offsetof(UnkSwarmMember_l6587bc, field_0x180) == 0x180, "member layout");

struct UnkSwarmParams_l6587bc {
    u8 pad_0x0[0x488];
    f32 speed_min;  // 0x488
    f32 speed_max;  // 0x48c
};

struct UnkSwarmController_l6587bc {
    u8 pad_0x0[0x560];
    UnkSwarmParams_l6587bc* params;  // 0x560
};

static inline f32 RandRange_l6587bc(f32 lo, f32 hi) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * t;
}

// Stores (x, y, z) scaled to unit length (kept finite for a zero vector).
static inline void SetNormalized_l6587bc(f32* out, f32 x, f32 y, f32 z) {
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
    out[0] = inv * x;
    out[1] = inv * y;
    out[2] = inv * z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71006587bc(UnkSwarmController_l6587bc* self, UnkSwarmMember_l6587bc* member) {
    if (member->field_0x180 && fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
        fn_7100655280(reinterpret_cast<UnkRedenkiController_b55*>(self), reinterpret_cast<UnkTarget*>(member));

    UnkSwarmParams_l6587bc* params = self->params;
    f32 speed = RandRange_l6587bc(params->speed_min, params->speed_max);
    member->speed = speed;
    member->target_speed = speed;

    f32 radius = member->orbit_radius;
    f32 angle = member->orbit_angle;
    f32 x = radius * cosf(angle);
    f32 y = radius * sinf(angle);

    f32 room = member->area->radius - radius;
    f32 reach = room < 15.0f ? room : 15.0f;
    f32 distance = RandRange_l6587bc(0.0f, reach);
    f32 heading = RandRange_l6587bc(0.0f, 6.2831855f);
    f32 dx = cosf(heading) * distance;
    f32 dy = sinf(heading) * distance;
    SetNormalized_l6587bc(member->direction, x + dx, y + dy, 0.0f);
}
