// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the chase step. A point near the target
// ends it (fn_710073f7bc with 1); otherwise the timers run down, the goal is refreshed when the retarget timer
// runs out, and a target within the tuned band along the path starts state 1. Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/SLogicPathNode.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2553.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkSwarmPoint_b65a4 {
    u64 value;
};

struct UnkSwarmRecord_b65a4 {
    bool active;
};

struct UnkPathPoints;

// Read as one 64-bit word: the area in the low half, the index in the high half.
struct UnkRedenkiPathPos_l65adec {
    u64 bits;
    s32 Area() const { return (s32)bits; }
    u32 Index() const { return (u32)(bits >> 32); }
};

// The swarm's target as fn_7100739c64 gives it: its place on the path and its distance from that point.
struct UnkRedenkiTarget_l65adec {
    u8 pad_0x0[0x50];
    UnkRedenkiPathPos_l65adec pathPos;  // 0x50
    u8 pad_0x58[0xc];
    f32 pathOffset;  // 0x64
};

struct UnkRedenkiArea_l65adec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual s32 vf_0x28();
};

struct UnkRedenkiTuning_l65adec {
    u8 pad_0x0[0x58];
    bool field_0x58;
    u8 pad_0x59[0x47c - 0x59];
    f32 farDistance;  // 0x47c
    f32 retargetTime;  // 0x480
    u8 pad_0x484[0x490 - 0x484];
    f32 nearDistance;  // 0x490
};

struct UnkRedenkiPath_l65adec {
    u8 pad_0x0[0x10];
    u32 count;  // 0x10
};

struct UnkRedenkiPlayerState_l65adec {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkRedenkiPlayerLogic_l65adec {
    u8 pad_0x0[0x2d08];
    UnkRedenkiPlayerState_l65adec* state;  // 0x2d08
};

struct UnkRedenkiPlayer_l65adec {
    u8 pad_0x0[0x208];
    UnkRedenkiPlayerLogic_l65adec* logic;  // 0x208

    bool InState6() const { return logic && logic->state && logic->state->state == 6; }
};

struct UnkGameFlags_l65adec {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    bool field_0xc5;
};

struct UnkRedenkiController_l65adec {
    void** vtable;
    u8 pad_0x8[0x80 - 0x8];
    UnkRedenkiArea_l65adec* area;  // 0x80
    u8 pad_0x88[0x2c8 - 0x88];
    UnkRedenkiPath_l65adec path;  // 0x2c8
    u8 pad_0x2dc[0x31c - 0x2dc];
    UnkSwarmRecord_b65a4 record;  // 0x31c
    u8 pad_0x31d[0x534 - 0x31d];
    f32 retargetTimer;  // 0x534
    f32 cooldown;  // 0x538
    u8 pad_0x53c[0x540 - 0x53c];
    f32 field_0x540;
    u8 pad_0x544[0x560 - 0x544];
    UnkRedenkiTuning_l65adec* tuning;  // 0x560

    void vf_0x2c0(const UnkVec3Result* goal, UnkRedenkiPath_l65adec* path, bool lowered) {
        reinterpret_cast<void (*)(UnkRedenkiController_l65adec*, const UnkVec3Result*, UnkRedenkiPath_l65adec*, bool)>(
            vtable[0x2c0 / 8])(this, goal, path, lowered);
    }
    void SetState(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkRedenkiController_l65adec*, u32, u32)>(vtable[0x2e8 / 8])(this, a, b);
    }
    bool vf_0x300() { return reinterpret_cast<bool (*)(UnkRedenkiController_l65adec*)>(vtable[0x300 / 8])(this); }
};

void fn_710065adec(UnkRedenkiController_l65adec* self, f32 dt) {
    bool leave = fn_710065634c(reinterpret_cast<UnkDaivoSwarm_b0012*>(self));
    UnkParent_71007455dc* parent = fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self));
    UnkRedenkiTarget_l65adec* target = reinterpret_cast<UnkRedenkiTarget_l65adec*>(parent);
    if (!self->vf_0x300()) {
        UnkSwarmPoint_b65a4 point;
        UnkVec3Result pos = fn_7100745e24(parent);
        point.value = 0;
        if (fn_710073fea8(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self), &pos, &point, 50.0f)) {
            fn_7100737d4c(&self->record, &point);
            self->record.active = true;
            fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
            return;
        }
    }

    self->cooldown -= dt;
    UnkRedenkiTuning_l65adec* tuning = self->tuning;
    f32 farDistance = tuning->farDistance;
    f32 nearDistance = tuning->nearDistance;
    self->retargetTimer -= dt;
    if (self->retargetTimer < 0.0f || leave) {
        self->retargetTimer = tuning->retargetTime;
        UnkVec3Result goal = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
        bool lowered = false;
        if (!self->tuning->field_0x58) {
            auto* player = reinterpret_cast<UnkRedenkiPlayer_l65adec*>(fn_7100dec788(g_unk_7101d47310));
            auto* flags = reinterpret_cast<UnkGameFlags_l65adec*>(g_unk_7101d47310);
            lowered = player && player->InState6() && !flags->field_0xc5 && flags->field_0xc4;
        }
        self->vf_0x2c0(&goal, &self->path, lowered);
    }

    u32 last = self->path.count - 1;
    UnkRedenkiPathPos_l65adec pos = target->pathPos;
    u32 index = pos.Index();
    f32 distance;
    f32 along;
    if (self->area->vf_0x28() != pos.Area() || self->path.count <= index || self->path.count <= last)
        distance = 3.40282347e+38f;
    else if (along = fn_71010598e0(reinterpret_cast<UnkPathPoints*>(&self->path), index, last), index <= last)
        distance = along + target->pathOffset;
    else
        distance = along - target->pathOffset;
    if (distance < farDistance && distance > nearDistance && self->cooldown < 0.0f) {
        self->field_0x540 = target->pathOffset;
        self->SetState(1, 0);
    }
}
