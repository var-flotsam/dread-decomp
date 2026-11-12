// CRedenkiSwarmControllerComponent: when fn_7100652554 allows it, takes the position of the object at
// +0x470 as the swarm's target (+0x548, with the mode at +0x544 set to 1); otherwise calls
// vf_0x2e8 with (0, 0). Names and layouts are placeholders.
#include "game/UnkTU1161.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable (as in
// src/game_lo/fn_71007455dc.cpp).
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkParent_71007455dc;

struct UnkSwarmController {
    virtual void vf_0x0();
    u8 pad_0x8[0x468];
    UnkParent_71007455dc* source;  // 0x470
    u8 pad_0x478[0xcc];
    u32 mode;  // 0x544
    f32 target_x;
    f32 target_y;
    f32 target_z;
};

bool fn_7100652554(UnkSwarmController* self);

static inline void CallSlot(UnkSwarmController* self, u32 a, u32 b) {
    (*reinterpret_cast<void (***)(UnkSwarmController*, u32, u32)>(self))[0x2e8 / 8](self, a, b);
}

void fn_71006524d8(UnkSwarmController* self) {
    if (fn_7100652554(self)) {
        UnkVec3Result p = fn_7100745e24(self->source);
        self->mode = 1;
        self->target_x = p.x;
        self->target_y = p.y;
        self->target_z = p.z;
        return;
    }
    CallSlot(self, 0, 0);
}
