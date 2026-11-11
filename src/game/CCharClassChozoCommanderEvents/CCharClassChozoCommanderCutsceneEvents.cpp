// The Chozo commander's cutscene events (units 1752 and 1753 of tools/tu_map.py, the names from
// reflection: CCharClassChozoCommanderDestroySentenceSpheresEvent and
// CCharClassChozoCommanderLaunchKiCounterCutsceneEvent). Each finds the player's actor through the
// global instance (0x108 bytes, created on first use) and, when its component at +0x240 is the
// commander AI component, hands it to the commander's action (fn_71004565f0, fn_71004554d8).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "types.h"

using base::reflection::CClass;

extern char* g_unk_71726b2398;

struct UnkComponent;
void fn_71004565f0(UnkComponent* component);

struct UnkCommanderComponent {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkCommanderComponent*)>(vtable[0])(this); }
};
struct UnkCommanderActor {
    u8 unk_0x0[0x240];
    UnkCommanderComponent* component;
};
struct UnkCommanderPlayer {
    u8 unk_0x0[0x10];
    UnkCommanderActor* actor;
};
struct UnkCommanderInstance {
    u8 unk_0x0[0x100];
    UnkCommanderPlayer* player;
};

__attribute__((always_inline)) static inline UnkCommanderComponent* CommanderComponent() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    UnkCommanderPlayer* player = reinterpret_cast<UnkCommanderInstance*>(instance)->player;
    if (!player)
        return nullptr;
    UnkCommanderActor* actor = player->actor;
    if (!actor)
        return nullptr;
    UnkCommanderComponent* component = actor->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710044e708()))
        return nullptr;
    return component;
}

void fn_7100b9e1fc() {
    if (UnkCommanderComponent* component = CommanderComponent())
        fn_71004565f0(reinterpret_cast<UnkComponent*>(component));
}

void fn_7100b9e7dc() {
    if (UnkCommanderComponent* component = CommanderComponent())
        fn_71004554d8(reinterpret_cast<UnkComponent*>(component));
}
