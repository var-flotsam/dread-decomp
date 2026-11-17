// CSwarmControllerComponent's unit: counts down the timer of every active member of one swarm group by `dt`,
// and hands each member whose timer runs out to fn_710074b680. Names and layouts are placeholders.
#include "game/UnkTU1159.h"
#include "types.h"

// A swarm member as this function sees it: its countdown timer.
struct UnkSwarmMember_l74b57c {
    u8 pad_0x0[0x154];
    f32 timer;  // 0x154
};

// One group of swarm members (0x148 bytes).
struct UnkSwarmGroup_l74b57c {
    u8 pad_0x0[0x24];
    bool enabled;  // 0x24
    u8 pad_0x25[0x93];
    UnkSwarmMember_l74b57c** members;  // 0xb8
    u32 memberCount;                    // 0xc0
    u8 pad_0xc4[0x84];
};
static_assert(sizeof(UnkSwarmGroup_l74b57c) == 0x148, "group size");

struct UnkSwarm_l74b57c {
    u8 pad_0x0[0x50];
    UnkSwarmGroup_l74b57c* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 groupCount;  // 0x110

    // Group i, or null past the end.
    UnkSwarmGroup_l74b57c* Get(s32 i) {
        if (i < groupCount && groups)
            return &groups[i];
        return nullptr;
    }
};

struct UnkSwarmRef_l74b57c {
    u8 pad_0x0[0x10];
    UnkSwarm_l74b57c* swarm;  // 0x10
};

struct UnkSwarmParams_l74b57c {
    u8 pad_0x0[0xf0];
    f32 rateA;  // 0xf0
    f32 rateB;  // 0xf4
};

struct UnkSwarmController_l74b57c {
    u8 pad_0x0[0xe8];
    UnkSwarmRef_l74b57c* ref;  // 0xe8
    u8 pad_0xf0[0xf0];
    UnkSwarmParams_l74b57c* params;  // 0x1e0

    UnkSwarm_l74b57c* Swarm() { return ref ? ref->swarm : nullptr; }
};

void fn_710074b680(UnkSwarmController_l74b57c* self, UnkSwarmMember_l74b57c* member, void* arg2, u32 arg3);

void fn_710074b57c(UnkSwarmController_l74b57c* self, f32 dt, s32 index) {
    UnkSwarmGroup_l74b57c* group = self->Swarm()->Get(index);
    if (!group || !(self->params->rateA > 0.0f) || !(self->params->rateB > 0.0f) || !group->enabled)
        return;
    for (u64 i = 0; i < group->memberCount; ++i) {
        UnkSwarmMember_l74b57c* member = group->members[i];
        if (!fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member)))
            continue;
        f32 before = member->timer;
        member->timer = before - dt;
        if (before > 0.0f && member->timer <= 0.0f)
            fn_710074b680(self, member, nullptr, 0);
    }
}
