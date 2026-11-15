// CBossRushSurvivalConfiguration's unit (unit 2544): the actor a stage kind refers to (the team manager's
// current actor, else one looked up from a manager by the stage), or null when its state is 2.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2547.h"
#include "types.h"

extern char* g_unk_71726b2398;

struct UnkActorState_dca218 {
    u8 pad_0x0[0x70];
    s32 value;  // 0x70
};

struct UnkActor_dca218 {
    u8 pad_0x0[0x1b8];
    UnkActorState_dca218* state;  // 0x1b8
};

struct UnkActorHolder_dca218 {
    u8 pad_0x0[0x10];
    UnkActor_dca218* actor;  // 0x10
};

struct UnkTeamManager_dca218 {
    u8 pad_0x0[0x100];
    UnkActorHolder_dca218* holder;  // 0x100
};

struct UnkNodeResult_dca218 {
    u8 pad_0x0[0x30];
    UnkActor_dca218* actor;  // 0x30
};

struct UnkNodeSet_dca218;
struct UnkSurvivalConfiguration_dca218 {
    u8 pad_0x0[0x100];
    u32 stage;  // 0x100
};

UnkNodeResult_dca218* fn_7100475564(UnkNodeSet_dca218* self);

static inline UnkActor_dca218* Visible(UnkActor_dca218* actor) {
    if (!actor || !actor->state)
        return nullptr;
    return actor->state->value == 2 ? nullptr : actor;
}

UnkActor_dca218* fn_7100dca218(UnkSurvivalConfiguration_dca218* self) {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    UnkActorHolder_dca218* holder = reinterpret_cast<UnkTeamManager_dca218*>(instance)->holder;
    if (holder && holder->actor)
        return Visible(holder->actor);
    u32 stage = self->stage;
    if (stage == 8 || stage == 5) {
        UnkManager_71726af468* manager = g_unk_71726af468;
        if (!manager) {
            manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
            fn_7100da7e2c(manager);
            g_unk_71726af468 = manager;
        }
        UnkNodeResult_dca218* node = fn_7100475564(reinterpret_cast<UnkNodeSet_dca218*>(reinterpret_cast<char*>(manager) + 0x228));
        if (!node || !node->actor)
            return nullptr;
        return Visible(node->actor);
    }
    if ((stage | 8) == 0xb) {
        UnkDoorRegistry_b61* registry = g_unk_71726bee10;
        if (!registry) {
            registry = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
            fn_7100ddb6dc(registry);
            g_unk_71726bee10 = registry;
        }
        UnkCentralUnit* unit = fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(registry));
        if (!unit)
            return nullptr;
        UnkActorHolder_dca218** slot = reinterpret_cast<UnkActorHolder_dca218**>(
            fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(reinterpret_cast<char*>(unit) + 0x438)));
        UnkActorHolder_dca218* link = *slot;
        if (!link || !link->actor)
            return nullptr;
        return Visible(link->actor);
    }
    return nullptr;
}
