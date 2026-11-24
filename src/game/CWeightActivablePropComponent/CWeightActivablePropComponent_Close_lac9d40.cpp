// CWeightActivablePropComponent's unit (tools/tu_map.py): closes the prop. Gives the owner's weak target the
// "closed" id, turns the "collision_close" and "player_detector" colliders back on and, when the owner's
// animation (+0x220) has the closing action, sends it a request with mode 1 (as src/game_s2/UnkSpitSend.h).
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CWeightActivablePropComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkWeakTarget;

// "closed", the third of the unit's names (g_unk_7172755b90 holds "collision_close", then "player_detector").
extern base::global::CStrId g_unk_7172755ba0 __attribute__((visibility("hidden")));

struct UnkWeightPropAction_ac9d40 {
    u8 pad_0x0[0x14];
    u32 state;  // 0x14
    u8 pad_0x18[0x48 - 0x18];
    u64 next;  // 0x48
};

struct UnkWeightPropOwner_ac9d40 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collisions;  // 0x2a0
};

struct UnkWeightProp_ac9d40 {
    u8 pad_0x0[0x30];
    UnkWeightPropOwner_ac9d40* owner;  // 0x30
};

static inline void Enable(UnkCollisionContainer* collisions, const base::global::CStrId* name) {
    if (fn_71008640a4(collisions, name))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collisions, name)), true);
}

void fn_7100ac9d40(UnkWeightProp_ac9d40* self) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg next_mode;
    base::reflection::UnkReflTypeIdArg next_mask;
    base::reflection::UnkReflTypeIdArg idle_mode;
    base::reflection::UnkReflTypeIdArg idle_mask;
    base::reflection::UnkReflTypeIdArg active_mode;
    base::reflection::UnkReflTypeIdArg active_mask;
start:
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, g_unk_7172755ba0.data->Key());
    UnkCollisionContainer* collisions = self->owner->collisions;
    Enable(collisions, &(&g_unk_7172755b90)[0]);
    Enable(collisions, &(&g_unk_7172755b90)[1]);

    UnkStruct_7101176ea8* animation = self->owner->animation;
    u64 id = 0x6a17390fc912b900;
    const u64 kind = 0xf4061ff25a2163d6;
    auto* action = static_cast<UnkWeightPropAction_ac9d40*>(fn_7101176d34(animation, reinterpret_cast<void*>(id)));
    if (!action)
        return;
    if (fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self))) {
        active_mode.id = 1;
        active_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, id, kind, &active_mode, &g_unk_710164b958, &g_unk_7172769740, &active_mask);
    } else if (action->state == 2) {
        id = action->next;
        next_mode.id = 1;
        next_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, id, kind, &next_mode, &g_unk_710164b958, &g_unk_7172769740, &next_mask);
    } else {
        idle_mode.id = 1;
        idle_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, id, kind, &idle_mode, &g_unk_710164b958, &g_unk_717276974c, &idle_mask);
    }
}
