#pragma once

// The robot soldier attacks' shared view (CChozoRobotSoldierDashSlashAttack_b49.cpp,
// CChozoRobotSoldierUppercutAttack_b49.cpp): the attack's actor, its CChozoRobotSoldierAIComponent
// (+0x240) and the state tests on it, the lazily created manager (g_unk_71726af468) whose +0x2a0 is
// the actor allowed to attack, and the target's life (+0x208). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

struct UnkSelf;
u32 fn_7100360b84(UnkSelf* self);
bool fn_71009e12d0(void*);
bool fn_7101136308(void*);
bool fn_7101161088(void*);

struct UnkRobotSoldierAI_b49 {
    void** vtable;
    u8 pad_0x8[0x37];
    u8 field_0x3f;  // a flag (returned as a byte)
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkRobotSoldierAI_b49*)>(vtable[0])(this);
    }
};
bool fn_7100471278(UnkRobotSoldierAI_b49* self);
bool fn_71004718fc(UnkRobotSoldierAI_b49* self);
bool fn_7100472fa4(UnkRobotSoldierAI_b49* self);
bool fn_71004744a4(UnkRobotSoldierAI_b49* self);
bool fn_710110a0a4(UnkRobotSoldierAI_b49* self);

struct UnkRobotSoldierActor_b49 {
    u8 pad_0x0[0x240];
    UnkRobotSoldierAI_b49* ai;
    u8 pad_0x248[0x10];
    void* field_0x258;
};

struct UnkRobotSoldierManager_b49 {
    u8 pad_0x0[0x2a0];
    UnkRobotSoldierActor_b49* actor;
};

struct UnkRobotSoldierState_b49 {
    u8 pad_0x0[0x10];
    u32 state;
};
struct UnkRobotSoldierLife_b49 {
    u8 pad_0x0[0x2d08];
    UnkRobotSoldierState_b49* state;
};
struct UnkRobotSoldierTarget_b49 {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x48];
    UnkRobotSoldierLife_b49* field_0x208;
};

struct UnkDashSlashAttack_b49 {
    void** vtable;
    UnkRobotSoldierActor_b49* actor;
};

static inline UnkManager_71726af468* GetRobotSoldierManager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}
