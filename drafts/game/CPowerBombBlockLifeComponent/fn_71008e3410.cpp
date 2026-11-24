// CPowerBombBlockLifeComponent's unit (tools/tu_map.py): restores a destroyed block from the
// blackboard: when the scenario's section says "Dead", breaks the model's parts, makes the "_BREAK"
// colliders solid, plays the break animation and sends the "destroyed" id. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_7100233684;
struct UnkStruct_7101176ea8;
struct UnkWeakTarget;

// A string id's entry as this unit's functions see it (src/game/CPowerBombBlockLifeComponent).
struct UnkPowerBombNameEntry_ovb0127 {
    u8 pad_0x0[0x10];
    base::global::CRntString buffer;  // 0x10
    u8 pad_0x40[0x4];
    union {
        u32 uses;  // 0x44
    };
};

struct UnkPowerBombCollider_ovb0127 {
    u8 pad_0x0[0x20];
    UnkPowerBombNameEntry_ovb0127* name;  // 0x20
};

struct UnkPowerBombColliders_ovb0127 {
    u8 pad_0x0[0x50];
    u32 count;  // 0x50
};

struct UnkPowerBombBlockLifeParts_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStruct_7100233684* owner;  // 0x30
};

void CPowerBombBlockLifeComponent_SwitchModelToBreakParts(UnkPowerBombBlockLifeParts_ovb0127* self);
bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);

extern UnkProperty g_unk_71727325b0;  // "Dead"
extern CStrId g_unk_7172732638 __attribute__((visibility("hidden")));  // "destroyed"
__attribute__((visibility("hidden"))) extern const u64 g_unk_710164b198;

struct UnkPowerBombRestoreOwner_l8e3410 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkPowerBombColliders_ovb0127* colliders;  // 0x2a0
};

struct UnkPowerBombBlockLifeRestore_l8e3410 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPowerBombRestoreOwner_l8e3410* owner;  // 0x30
    u8 pad_0x38[0x250 - 0x38];
    UnkStrIdTable properties;  // 0x250
};

void fn_71008e3410(UnkPowerBombBlockLifeRestore_l8e3410* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section;
    {
        CStrId scenario = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        section = fn_71010472e0(blackboard, scenario);
    }
    if (!section)
        return;
    bool dead = false;
    u64 index = 0;
    u32 count = self->properties.count;
    if (count) {
        CStrId* keys = self->properties.keys;
        base::global::UnkStringData* key = g_unk_71727325b0.name.data;
        for (; index != count; ++index) {
            if (keys[index].data == key)
                break;
        }
    }
    bool found = fn_710006d11c(section, &self->properties.values[index].id, &dead);
    if (!dead || !found)
        return;

    CPowerBombBlockLifeComponent_SwitchModelToBreakParts(reinterpret_cast<UnkPowerBombBlockLifeParts_ovb0127*>(self));
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, g_unk_7172732638.data->Key());

    if (UnkPowerBombColliders_ovb0127* colliders = self->owner->colliders) {
        for (u32 i = 0; i < colliders->count; ++i) {
            UnkPowerBombCollider_ovb0127* collider = fn_710086420c(colliders, i);
            UnkPowerBombNameEntry_ovb0127* const& name = collider->name;
            ++name->uses;
            bool broken;
            if (name != reinterpret_cast<UnkPowerBombNameEntry_ovb0127*>(&g_unk_7101d47108->empty) &&
                CRntString_Contains(&name->buffer, "_BREAK"))
                broken = true;
            else
                broken = false;
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), broken);
        }
    }

    if (void* animation = self->owner->animation) {
        const u64 hash = 0xc567047b7a4bf0cf;
        if (fn_7101176e78(static_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash), nullptr)) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_717276974c, &mask);
        }
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b198, false);
}
