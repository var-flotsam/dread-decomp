// CWeightActivablePropComponent's unit (tools/tu_map.py): opens the prop. Gives the owner's weak target the
// "opened" id, turns off the "collision_close" and "player_detector" colliders and, when the owner has its
// animation object (+0x220), sends it a request (as src/game_s2/UnkSpitSend.h) after hooking a callback up
// to it; without one it finishes at once. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CWeightActivablePropComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU341.h"
#include "game/UnkTU372.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeightPropComponent_b43a3;
void fn_7100aca378(UnkWeightPropComponent_b43a3* self);
u32 fn_7100acb254(char* p0, char* p1);
// "opened", the last of the unit's names (g_unk_7172755b90 holds "collision_close", then "player_detector").
extern base::global::CStrId g_unk_7172755ba8 __attribute__((visibility("hidden")));

struct UnkWeightPropOwner_aca09c {
    u8 pad_0x0[0x220];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collisions;  // 0x2a0
};

struct UnkWeightPropParams_aca09c {
    u8 pad_0x0[0x58];
    bool hook;  // 0x58
};

struct UnkWeightPropLink_aca09c {
    u8 pad_0x0[0x8];
    void* context;
    u32 (*callback)(char*, char*);
};

struct UnkWeightProp_aca09c {
    u8 pad_0x0[0x30];
    UnkWeightPropOwner_aca09c* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    UnkWeightPropParams_aca09c* params;  // 0x48
    u8 pad_0x50[0x1e0 - 0x50];
    UnkWeightPropLink_aca09c link;  // 0x1e0
    UnkWeakHandle* handle;          // 0x1f8
};
static_assert(__builtin_offsetof(UnkWeightProp_aca09c, handle) == 0x1f8, "UnkWeightProp_aca09c layout");

void fn_7100aca09c(UnkWeightProp_aca09c* self) {
    base::reflection::UnkReflTypeIdArg idle_mode;
    base::reflection::UnkReflTypeIdArg idle_mask;
    base::reflection::UnkReflTypeIdArg open_mode;
    base::reflection::UnkReflTypeIdArg open_mask;
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, g_unk_7172755ba8.data->Key());
    UnkCollisionContainer* collisions = self->owner->collisions;
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collisions, &(&g_unk_7172755b90)[0])), false);
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collisions, &(&g_unk_7172755b90)[1])), false);

    UnkWeightPropOwner_aca09c* owner = self->owner;
    UnkWeakTarget* animation = owner->animation;
    if (!animation) {
        fn_7100232c68(owner);
        fn_7100aca378(reinterpret_cast<UnkWeightPropComponent_b43a3*>(self));
        return;
    }
    const u64 hash = 0xa0e33be014f44bd7;
    const u64 kind = 0xf4061ff25a2163d6;
    if (!fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self))) {
        idle_mode.id = 1;
        idle_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash, kind, &idle_mode, &g_unk_710164b958, &g_unk_717276974c, &idle_mask);
        return;
    }
    if (self->params->hook) {
        UnkWeightPropLink_aca09c* link = &self->link;
        link->context = self;
        link->callback = fn_7100acb254;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            ResetRef(&self->handle);
        }
        fn_7100316018(animation, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    open_mode.id = 1;
    open_mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, kind, &open_mode, &g_unk_710164b958, &g_unk_7172769740, &open_mask);
}
