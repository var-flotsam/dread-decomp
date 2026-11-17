// CChozoRobotSoldierAIComponent unit: whether this robot soldier may start an attack: its owner's state,
// the tuning's switch or its own flag, its timers and state, no other registered soldier with its attack
// timer started or busy (fn_71011057fc), and, when there are several, not the one the manager
// (g_unk_71726af468, +0x2a0) holds. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAttackComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkRobotSoldierTuning_l473450 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;  // the attack timer's limit
    u8 pad_0x30[0x24];
    bool field_0x54;
};
extern UnkRobotSoldierTuning_l473450 g_unk_71726cc6d8;  // reached through the GOT

struct UnkSoldierOwnerState_l473450 {
    u8 pad_0x0[0x70];
    u32 state;
};

struct UnkSoldierActor_l473450 {
    u8 pad_0x0[0x1b8];
    UnkSoldierOwnerState_l473450* field_0x1b8;
    u8 pad_0x1c0[0x98];
    void* field_0x258;
};

// A node of the state machine: its state at +0x8.
struct UnkSoldierStateNode_l473450 {
    u8 pad_0x0[0x8];
    void* state;
};

struct UnkRobotSoldierAI_l473450 {
    u8 pad_0x0[0x30];
    UnkSoldierActor_l473450* owner;
    u8 pad_0x38[0x1124 - 0x38];
    f32 field_0x1124;
    u8 pad_0x1128[0x1170 - 0x1128];
    u32 field_0x1170;
    UnkSoldierStateNode_l473450* field_0x1178;
    u8 pad_0x1180[0x149a8 - 0x1180];
    u8 field_0x149a8[0x14b18 - 0x149a8];  // a state
    f32 field_0x14b18;
    f32 field_0x14b1c;  // the attack timer
    u8 pad_0x14b20[0x27790 - 0x14b20];
    bool field_0x27790;
};
static_assert(__builtin_offsetof(UnkRobotSoldierAI_l473450, field_0x27790) == 0x27790,
              "UnkRobotSoldierAI_l473450 layout");

struct UnkSoldierEntry_l473450 {
    u8 pad_0x0[0x10];
    UnkRobotSoldierAI_l473450* ai;
};

struct UnkSoldierList_l473450 {
    UnkSoldierEntry_l473450** data;
    u32 count;

    UnkSoldierEntry_l473450** begin() { return data; }
    UnkSoldierEntry_l473450** end() { return data + count; }
};

struct UnkRobotSoldierManager_l473450 {
    u8 pad_0x0[0x2a0];
    UnkSoldierActor_l473450* actor;
};

extern char g_unk_71726cca00[] __attribute__((visibility("hidden")));

static inline UnkSoldierList_l473450* Soldiers_l473450() {
    return reinterpret_cast<UnkSoldierList_l473450*>(
        fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(g_unk_71726cca00)));
}

static inline UnkManager_71726af468* Manager_l473450() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

// Whether the attack timer is running (as fn_7100472fa4).
inline bool AttackTimerRunning_l473450(const UnkRobotSoldierAI_l473450* ai) {
    f32 t = ai->field_0x14b1c;
    return (t >= 0.0f) & (t < g_unk_71726cc6d8.field_0x2c);
}

bool fn_7100473450(UnkRobotSoldierAI_l473450* self) {
    UnkSoldierActor_l473450* owner = self->owner;
    bool ready = !owner->field_0x1b8 || owner->field_0x1b8->state != 2;
    if (!g_unk_71726cc6d8.field_0x54 && !self->field_0x27790)
        ready = false;
    if (!ready)
        return false;
    if (owner->field_0x258 && fn_7101136308(owner->field_0x258))
        return false;
    f32 t = self->field_0x1124;
    if (self->field_0x14b18 > -1.0f) {
        if (!(t > 0.5f))
            return false;
    }
    if (t < 1.5f && self->field_0x1170 != 0 && self->field_0x1178->state == self->field_0x149a8)
        return false;
    for (UnkSoldierEntry_l473450* entry : *Soldiers_l473450()) {
        UnkRobotSoldierAI_l473450* other = entry ? entry->ai : nullptr;
        if (other == self)
            continue;
        if (AttackTimerRunning_l473450(other) || other->field_0x14b1c >= g_unk_71726cc6d8.field_0x2c ||
            fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(other)))
            return false;
    }
    if (Soldiers_l473450()->count < 2)
        return true;
    return reinterpret_cast<UnkRobotSoldierManager_l473450*>(Manager_l473450())->actor != self->owner;
}
