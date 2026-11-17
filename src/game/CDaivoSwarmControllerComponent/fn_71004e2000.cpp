// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): when the swarm is within 200 units of
// its path's last point, sends it (fn_7100656f3c) along x, away from the side the target is moving to.
// Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp defines it.
struct UnkZeroVec_b0012 {
    f32 x, y, z;
    UnkZeroVec_b0012() : x(0.0f), y(0.0f), z(0.0f) {}
};

class UnkDaivoSwarm_b0012;
struct UnkSwarmPath_b0012;

// As src/game/CDaivoSwarmControllerComponent/CDaivoSwarmControllerComponent_Swarm_ovb0012.cpp declares it.
void fn_7100656f3c(UnkDaivoSwarm_b0012* self, UnkSwarmPath_b0012* path, const UnkVec3Result* dir, const f32* speed,
                   bool* out, const UnkZeroVec_b0012* offset, f32 radius, f32 limit);

// The object the swarm follows: its velocity at +0x70.
struct UnkSwarmTarget_l4e2000 {
    u8 pad_0x0[0x70];
    f32 velocity[3];  // 0x70
};

struct UnkPoint_l4e2000 {
    f32 x, y, z;
};

// A point of the swarm's path (0x20 bytes); its position comes from vf_0x18.
struct UnkPathPoint_l4e2000 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkPoint_l4e2000& Position();
    u8 pad_0x8[0x20 - 0x8];
};

struct UnkSwarmPath_l4e2000 {
    u8 pad_0x0[0x8];
    UnkPathPoint_l4e2000* points;  // 0x8
    u32 count;  // 0x10
};

struct UnkDaivoTuning_l4e2000 {
    u8 pad_0x0[0x484];
    f32 speed;  // 0x484
};

struct UnkDaivoSwarm_l4e2000 {
    u8 pad_0x0[0x2c8];
    UnkSwarmPath_l4e2000 path;  // 0x2c8
    u8 pad_0x2e0[0x395 - 0x2e0];
    bool moving;  // 0x395
    u8 pad_0x396[0x570 - 0x396];
    UnkDaivoTuning_l4e2000* tuning;  // 0x570
};


// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71004e2000(UnkDaivoSwarm_l4e2000* self) {
    UnkSwarmTarget_l4e2000* target = reinterpret_cast<UnkSwarmTarget_l4e2000*>(
        fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self)));
    UnkVec3Result pos = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
    if (self->path.count == 0)
        return;
    const UnkPoint_l4e2000& last = self->path.points[self->path.count - 1].Position();
    f32 dx = pos.x - last.x;
    f32 dy = pos.y - last.y;
    f32 dz = pos.z - last.z;
    if (!(dx * dx + dy * dy + dz * dz < 40000.0f))
        return;
    f32 vx = target->velocity[0];
    s32 side = (vx > 0.0f) - (vx < 0.0f);
    UnkVec3Result dir(-(f32)side, 0.0f, 0.0f);
    f32 speed = self->tuning->speed;
    bool arrived;  // unset in the original: fn_7100656f3c writes it
    UnkZeroVec_b0012 offset;
    fn_7100656f3c(reinterpret_cast<UnkDaivoSwarm_b0012*>(self), reinterpret_cast<UnkSwarmPath_b0012*>(&self->path),
                  &dir, &speed, &arrived, &offset, 75.0f, -1.0f);
    self->moving = true;
}
