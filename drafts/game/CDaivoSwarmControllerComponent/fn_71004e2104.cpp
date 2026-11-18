// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): the swarm's per-frame update. Hands
// over (fn_710073f7bc) when the target is reachable, re-plans (vf_0x2c0) on a timer, and attacks
// (vf_0x2e8 with 2) when the target closes in on the swarm.
// Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CLifeComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2553.h"
#include "types.h"

extern "C" f32 sqrtf(f32);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

bool fn_71006502f4(UnkDaivoSwarm_b0012* self, const UnkVec3Result* pos);
bool fn_710065244c(UnkDaivoSwarm_b0012* self);

#define UNK_VF8(n) virtual void vf_##n##_0(); virtual void vf_##n##_1(); virtual void vf_##n##_2(); \
    virtual void vf_##n##_3(); virtual void vf_##n##_4(); virtual void vf_##n##_5(); \
    virtual void vf_##n##_6(); virtual void vf_##n##_7();

// The object the swarm follows: its velocity at +0x70.
struct UnkSwarmTarget_l4e2104 {
    u8 pad_0x0[0x70];
    f32 velocity[3];  // 0x70
};

struct UnkSwarmPlayer_l4e2104 {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
};

struct UnkDaivoTuning_l4e2104 {
    u8 pad_0x0[0x47c];
    f32 farRange;  // 0x47c
    f32 replanTime;  // 0x480
    u8 pad_0x484[0x490 - 0x484];
    f32 nearRange;  // 0x490
};

struct UnkSwarmPath_l4e2104 {
    u8 data[0x8];
};

class UnkDaivoSwarm_l4e2104 {
public:
    // Slots 0x0-0x2b8 (88 entries).
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5) UNK_VF8(6) UNK_VF8(7)
    UNK_VF8(8) UNK_VF8(9) UNK_VF8(10)
    virtual void vf_0x2c0(const UnkVec3Result* pos, UnkSwarmPath_l4e2104* path, bool flag);
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(u8 state, bool arg);
    virtual void vf_0x2f0();
    virtual void vf_0x2f8();
    virtual bool vf_0x300();

    u8 pad_0x8[0x2c8 - 0x8];
    UnkSwarmPath_l4e2104 path;  // 0x2c8
    u8 pad_0x2d0[0x534 - 0x2d0];
    f32 replanTimer;  // 0x534
    f32 attackTimer;  // 0x538
    u8 pad_0x53c[0x540 - 0x53c];
    f32 targetDistance;  // 0x540
    u8 pad_0x544[0x570 - 0x544];
    UnkDaivoTuning_l4e2104* tuning;  // 0x570
};

#undef UNK_VF8

static inline UnkDaivoSwarm_b0012* AsSwarm(UnkDaivoSwarm_l4e2104* self) {
    return reinterpret_cast<UnkDaivoSwarm_b0012*>(self);
}

static inline f32 Square(f32 v) {
    return v * v;
}

void fn_71004e2104(UnkDaivoSwarm_l4e2104* self, f32 dt) {
    fn_710065634c(AsSwarm(self));
    UnkParent_71007455dc* parent = fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self));
    if (!self->vf_0x300()) {
        if (parent) {
            UnkVec3Result targetPos = fn_7100745e24(parent);
            if (fn_71006502f4(AsSwarm(self), &targetPos)) {
                fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
                return;
            }
        }
    }

    UnkVec3Result pos = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
    self->replanTimer -= dt;
    if (self->replanTimer < 0.0f) {
        self->replanTimer = self->tuning->replanTime;
        if (self->vf_0x300()) {
            bool flag = fn_710065244c(AsSwarm(self));
            self->vf_0x2c0(&pos, &self->path, flag);
        }
    }
    self->attackTimer -= dt;

    auto* player = reinterpret_cast<UnkSwarmPlayer_l4e2104*>(fn_7100dec788(g_unk_7101d47310));
    if (!player->life || fn_7101161088(player->life))
        return;

    UnkSwarmTarget_l4e2104* target = reinterpret_cast<UnkSwarmTarget_l4e2104*>(parent);
    UnkVec3Result targetPos = fn_7100745e24(parent);
    f32 dx = pos.x - targetPos.x;
    f32 dy = pos.y - targetPos.y;
    f32 dz = pos.z - targetPos.z;
    // Only while the target moves towards the swarm.
    if (dx * target->velocity[0] + dy * target->velocity[1] + dz * target->velocity[2] <= 0.0f)
        return;
    f32 distSq = dx * dx + dy * dy + dz * dz;
    UnkDaivoTuning_l4e2104* tuning = self->tuning;
    if (distSq < Square(tuning->nearRange) || (distSq < Square(tuning->farRange) && self->attackTimer < 0.0f)) {
        self->targetDistance = sqrtf(distSq);
        self->vf_0x2e8(2, false);
    }
}
