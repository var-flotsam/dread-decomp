// CSamusAlternativeActionPlayerComponent's unit (tools/tu_map.py, unit 1139): whether the alternative action
// may start: fn_7100711e00 allows it, no running slow motion, the owner's movement (+0x1f8) faces up enough,
// the gun is neither busy nor a grapple beam, no team-manager target, nothing in range of the door registry
// and no actor in the manager's list is in mode 1. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CGunComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkSlomoFlags.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2547.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkUnit2547;
void fn_7100ddba14(UnkUnit2547* self, f32* best, u32* best_id);

struct UnkAltActionPlayer_l712e04;
bool fn_7100711e00(UnkAltActionPlayer_l712e04* self);

struct UnkAltActionTarget_l712e04 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkAltActionMovement_l712e04 {
    u8 pad_0x0[0x128];
    UnkAltActionTarget_l712e04* target;  // 0x128
    u8 pad_0x130[0x154 - 0x130];
    f32 up_x;  // 0x154
    f32 up_y;
    f32 up_z;
    u8 pad_0x160[0x3a8 - 0x160];
    bool active;  // 0x3a8
};

struct UnkAltActionOwner_l712e04 {
    u8 pad_0x0[0x1f8];
    UnkAltActionMovement_l712e04* movement;  // 0x1f8
    u8 pad_0x200[0x2a8 - 0x200];
    UnkGunComponent* gun;  // 0x2a8
};

struct UnkAltActionPlayer_l712e04 {
    u8 pad_0x0[0x30];
    UnkAltActionOwner_l712e04* owner;  // 0x30
};

struct UnkAltActionNode_l712e04 {
    UnkActor* actor;
    UnkAltActionNode_l712e04* next;
};

struct UnkAltActionList_l712e04 {
    u8 pad_0x0[0x20];
    UnkAltActionNode_l712e04* first;  // 0x20
};

static inline char* TeamManager() {
    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    return manager;
}

static inline UnkDoorRegistry_b61* DoorRegistry() {
    UnkDoorRegistry_b61* registry = g_unk_71726bee10;
    if (!registry) {
        registry = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(registry);
        g_unk_71726bee10 = registry;
    }
    return registry;
}

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

bool fn_7100712e04(UnkAltActionPlayer_l712e04* self) {
    if (!fn_7100711e00(self))
        return false;
    UnkGameManager* game = g_unk_7101d47310;
    if (game->field_0xc8 != 0 || game->field_0x10c) {
        UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&game->field_0xc4);
        if (slomo->enabled && !slomo->paused)
            return false;
    }
    UnkAltActionOwner_l712e04* owner = self->owner;
    UnkAltActionMovement_l712e04* movement = owner->movement;
    if (movement && movement->active && movement->target && movement->target->field_0x10) {
        f32 up = movement->up_x * 0.0f + movement->up_y + movement->up_z * 0.0f;
        if (up < 0.98f)
            return false;
    }
    if (UnkGunComponent* gun = owner->gun) {
        if (fn_710084088c(reinterpret_cast<UnkSamusGunComponent*>(gun)))
            return false;
        UnkReflObject* beam = fn_71008407d8(gun);
        if (beam && fn_710009d50c(beam->vf_0x0(), fn_7100832034()))
            return false;
    }
    UnkAltActionTarget_l712e04* target = *reinterpret_cast<UnkAltActionTarget_l712e04**>(TeamManager() + 0x100);
    if (target && target->field_0x10)
        return false;
    f32 best = 0.0f;
    u32 best_id = 0;
    fn_7100ddba14(reinterpret_cast<UnkUnit2547*>(DoorRegistry()), &best, &best_id);
    if (best > 0.0f)
        return false;
    UnkAltActionList_l712e04* list = reinterpret_cast<UnkAltActionList_l712e04*>(Manager());
    for (UnkAltActionNode_l712e04* node = list->first; node; node = node->next) {
        if (fn_7100d7bea0(node->actor, 1))
            return false;
    }
    return true;
}
