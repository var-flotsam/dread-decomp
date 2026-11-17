// CCharClassTunnelTrapMorphballComponent's unit (tools/tu_map.py unit 1560): refreshes the trap's state:
// while it is not being reset, opens or closes it by its flag at +0x48 (colliders, the animation request
// and the navigation item's id), then turns one collider off when +0xd8 is set. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;

// The unit's collider names: one id (as CTunnelTrapMorphballComponent_Msg_b49a4.cpp declares it), then a pair.
extern void* g_unk_717274a6d8 __attribute__((visibility("hidden")));
extern CStrId g_unk_717274a6e0[2] __attribute__((visibility("hidden")));

struct UnkTunnelTrapOwner_la43a34 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collisions;  // 0x2a0
};

struct UnkTunnelTrap_la43a34 {
    u8 pad_0x0[0x30];
    UnkTunnelTrapOwner_la43a34* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 field_0x40[0x8];
    u8 open;  // 0x48
    u8 pad_0x49[0xd8 - 0x49];
    u8 field_0xd8;
};

// As CTunnelTrapMorphballComponent_b48.cpp and CTunnelTrapMorphballComponent_b36.cpp define them.
struct UnkTunnelTrap_b48;
struct UnkTunnelTrapMorphball;
bool fn_7100a43e7c(UnkTunnelTrap_b48*);
bool fn_7100a43f78(UnkTunnelTrap_b48*);
void fn_7100a44074(UnkTunnelTrapMorphball*);

static inline void SetCollider(UnkCollisionContainer* collisions, const CStrId* name, bool enabled) {
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collisions, name)), enabled);
}

void fn_7100a43a34(UnkTunnelTrap_la43a34* self) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg play_mode;
    base::reflection::UnkReflTypeIdArg play_mask;
    base::reflection::UnkReflTypeIdArg open_mode;
    base::reflection::UnkReflTypeIdArg open_mask;
    base::reflection::UnkReflTypeIdArg close_mode;
    base::reflection::UnkReflTypeIdArg close_mask;
start:
    self->open = fn_7100a43e7c(reinterpret_cast<UnkTunnelTrap_b48*>(self));
    bool resetting = fn_7100a43f78(reinterpret_cast<UnkTunnelTrap_b48*>(self));
    UnkStruct_7101176ea8* animation = self->owner->animation;
    void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner));
    if (!animation || !target)
        return;

    const u64 kind = 0xf4061ff25a2163d6;
    if (resetting) {
        fn_7100a44074(reinterpret_cast<UnkTunnelTrapMorphball*>(self));
    } else if (self->open) {
        if (UnkCollisionContainer* collisions = self->owner->collisions) {
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(collisions), true);
            if (UnkCollisionContainer* collisions = self->owner->collisions) {
                fn_71008659c4(collisions, 0x10000, 0x10, self->field_0x40);
                SetCollider(collisions, &g_unk_717274a6e0[1], true);
                SetCollider(collisions, &g_unk_717274a6e0[0], false);
            }
        }
        u64 id = 0xe43bbd07b9e307b9;
        if (fn_7101176e78(animation, reinterpret_cast<void*>(id), nullptr)) {
            play_mode.id = 0;
            play_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, id, kind, &play_mode, &g_unk_710164b968, &g_unk_7172769740, &play_mask);
        } else {
            open_mode.id = 0;
            open_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x6a17390fc912b900, kind, &open_mode, &g_unk_710164b968, &g_unk_717276974c,
                          &open_mask);
        }
        fn_71009c0f1c(target, 0xb15b31b835c17820);
        fn_71009c0974(target, true);
    } else {
        close_mode.id = 0;
        close_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, 0x6909d27d2c787fba, kind, &close_mode, &g_unk_710164b958, &g_unk_7172769740,
                      &close_mask);
        if (UnkCollisionContainer* collisions = self->owner->collisions) {
            fn_71008656d8(collisions, 0x10000, 0x10, self->field_0x40);
            SetCollider(collisions, &g_unk_717274a6e0[1], false);
            SetCollider(collisions, &g_unk_717274a6e0[0], true);
        }
        fn_71009c0f1c(target, 0x13ae275a1bcd6558);
        fn_71009c0974(target, false);
    }

    if (self->field_0xd8) {
        if (UnkCollisionContainer* collisions = self->owner->collisions)
            SetCollider(collisions, reinterpret_cast<const CStrId*>(&g_unk_717274a6d8), false);
    }
}
