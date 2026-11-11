// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): fn_7100859544 runs once (+0x340): it plays
// the actor's (+0x30) timeline key g_unk_710164ad60 when the player's actor (through the global
// instance, 0x108 bytes created on first use, as in
// src/game/CCharClassChozoCommanderEvents/CCharClassChozoCommanderCutsceneEvents.cpp) has a
// CGoliathXAIComponent at +0x240, else g_unk_710164ad68 (fn_7100d7c828). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::CClass;

extern char* g_unk_71726b2398;
extern u64 g_unk_710164ad60;
extern u64 g_unk_710164ad68;

struct UnkGunAIComponent_b52 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkGunAIComponent_b52*)>(vtable[0])(this); }
};
struct UnkGunPlayerActor_b52 {
    u8 unk_0x0[0x240];
    UnkGunAIComponent_b52* component;
};
struct UnkGunPlayer_b52 {
    u8 unk_0x0[0x10];
    UnkGunPlayerActor_b52* actor;
};
struct UnkGunInstance_b52 {
    u8 unk_0x0[0x100];
    UnkGunPlayer_b52* player;
};

__attribute__((always_inline)) static inline UnkGunAIComponent_b52* PlayerGoliathX_b52() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    UnkGunPlayer_b52* player = reinterpret_cast<UnkGunInstance_b52*>(instance)->player;
    if (!player)
        return nullptr;
    UnkGunPlayerActor_b52* actor = player->actor;
    if (!actor)
        return nullptr;
    UnkGunAIComponent_b52* component = actor->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_7100587ee4()))
        return nullptr;
    return component;
}

struct UnkSamusGun_b52 {
    u8 unk_0x0[0x30];
    UnkActor* actor;
    u8 unk_0x38[0x308];
    bool done;  // 0x340
};

void fn_7100859544(UnkSamusGun_b52* self) {
    if (self->done)
        return;
    self->done = true;
    if (!self->actor)
        return;
    if (PlayerGoliathX_b52())
        fn_7100d7c828(self->actor, &g_unk_710164ad60, false);
    else
        fn_7100d7c828(self->actor, &g_unk_710164ad68, false);
}
