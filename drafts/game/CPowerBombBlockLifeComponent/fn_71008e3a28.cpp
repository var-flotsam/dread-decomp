// CPowerBombBlockLifeComponent's unit (tools/tu_map.py): the block's death. Sends the "destroyed" id,
// makes the "_BREAK" colliders solid, stores "Dead" in the scenario's blackboard section, plays the
// break on the timeline and animation (or breaks the model's parts when neither plays it), then runs
// the base life component's handler. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_7100233684;
struct UnkStruct_7101176ea8;
struct UnkWeakTarget;
struct UnkLifeBase_A5_b63;
struct UnkLifeArg_A5_b63;
struct UnkHitInfo_A5_b63;

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
void fn_710116064c(UnkLifeBase_A5_b63* self, UnkLifeArg_A5_b63* arg, UnkHitInfo_A5_b63* info);

extern UnkProperty g_unk_71727325b0;  // "Dead"
extern CStrId g_unk_7172732638 __attribute__((visibility("hidden")));  // "destroyed"
__attribute__((visibility("hidden"))) extern const u64 g_unk_710164b1a0;

struct UnkPowerBombDieOwner_l8e3a28 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkPowerBombColliders_ovb0127* colliders;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkPowerBombBlockLifeDie_l8e3a28 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPowerBombDieOwner_l8e3a28* owner;  // 0x30
    u8 pad_0x38[0x250 - 0x38];
    UnkStrIdTable properties;  // 0x250
};

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_71008e3a28(UnkPowerBombBlockLifeDie_l8e3a28* self, u32 kind, UnkHitInfo_A5_b63* info) {
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

    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
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
        UnkStrIdTableValue* values = self->properties.values;
        bool dead = true;
        SetSectionBool(blackboard, section_id, reinterpret_cast<UnkPropertyName*>(&values[index].id), &dead);
    }

    // Play the break on the owner's timeline and animation; break the model's parts when neither plays it.
    UnkTimelineComponent* timeline = self->owner->timeline;
    void* animation = self->owner->animation;
    bool played = false;
    if (timeline || animation) {
        if (timeline) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164b1a0, &mask.id, false, -1.0f);
        }
        const u64 hash = 0xc567047b7a4bf0cf;
        if (animation &&
            fn_7101176e78(static_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash), nullptr)) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
            played = true;
        } else if (timeline && fn_7100a2c13c(timeline, &g_unk_710164b1a0)) {
            played = true;
        }
    }
    if (!played)
        CPowerBombBlockLifeComponent_SwitchModelToBreakParts(reinterpret_cast<UnkPowerBombBlockLifeParts_ovb0127*>(self));
    // The base handler takes the same arguments; the kind (w1) is a 32-bit value here.
    fn_710116064c(reinterpret_cast<UnkLifeBase_A5_b63*>(self), reinterpret_cast<UnkLifeArg_A5_b63*>(u64(kind)), info);
}
