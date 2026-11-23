// CItemLifeComponent unit (tools/tu_map.py unit 1379): when the scenario's blackboard section says the item
// is "Dead", shows the model's "_DEBRIS" parts and hides the others (but "Root"), turns on only the debris
// colliders, and, with no debris shown and no object found, takes the entity out; then switches off the
// entity's component fn_71008e0e54 finds. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CCoreXAIComponent.h"
#include "game/CInputComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU253.h"
#include "game/UnkTU341.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTU883.h"
#include "game/CDropComponent.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPowerBombCollider_ovb0127;
struct UnkPowerBombColliders_ovb0127;
struct UnkPowerBombPartList_ovb0127;
struct UnkPowerBombPart_ovb0127;
struct UnkHitInfo_A5_b63;
struct UnkActor;
struct UnkWeakTarget;
struct UnkItemLifeOwner_l8e0adc;


struct UnkItemLifePart_l8e0adc {
    UnkStringData* name;
};

struct UnkItemLifeModelResource_l8e0adc;

// The model's named parts (fn_71001c1440 counts them, fn_71001c1450 returns one).
static inline u32 PartCount(UnkItemLifeModelResource_l8e0adc* resource) {
    return fn_71001c1440(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource));
}
static inline UnkItemLifePart_l8e0adc* PartAt(UnkItemLifeModelResource_l8e0adc* resource, u32 index) {
    return reinterpret_cast<UnkItemLifePart_l8e0adc*>(
        fn_71001c1450(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource), index));
}

struct UnkItemLifeModelData_l8e0adc {
    u8 pad_0x0[0x138];
    UnkItemLifeModelResource_l8e0adc* resource;  // 0x138
};

struct UnkItemLifeModel_l8e0adc {
    u8 pad_0x0[0x50];
    UnkItemLifeModelData_l8e0adc* data;  // 0x50
};

struct UnkItemLifeCollider_l8e0adc {
    u8 pad_0x0[0x20];
    UnkStringData* name;  // 0x20
};

struct UnkItemLifeOwner_l8e0adc {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkItemLifeValue_l8e0adc {
    CStrId id;
    u64 field_0x8;
};

struct UnkPowerUpLifeBase_ovb0126 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkItemLifeOwner_l8e0adc* owner;  // 0x30
    u8 pad_0x38[0x268 - 0x38];
    UnkItemLifeValue_l8e0adc* values;  // 0x268
    u64* keys;                         // 0x270
    u32 count;                         // 0x278
    u8 pad_0x27c[0x290 - 0x27c];
    u8 field_0x290[0xa8];
    UnkHitInfo_A5_b63* field_0x338;  // 0x338

    // The value stored under `property`'s name (one past the end when there is none).
    UnkItemLifeValue_l8e0adc* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* ids = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (ids[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

static inline UnkItemLifeModel_l8e0adc* ModelOf(UnkItemLifeOwner_l8e0adc* owner) {
    return reinterpret_cast<UnkItemLifeModel_l8e0adc*>(
        fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(owner)));
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return CRntString_Contains(&name->String(), str);
}

void fn_71008e0adc(UnkPowerUpLifeBase_ovb0126* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool dead = false;
    bool found = fn_710006d11c(section, &self->Find(g_unk_7172732320)->id, &dead);
    if (!dead || !found)
        return;

    bool debris_shown = false;
    if (UnkItemLifeModel_l8e0adc* model = ModelOf(self->owner)) {
        if (UnkItemLifeModelData_l8e0adc* data = model->data) {
            for (u32 i = 0; i < PartCount(data->resource); ++i) {
                UnkItemLifePart_l8e0adc* part = PartAt(data->resource, i);
                if (NameHas(part->name, "_DEBRIS")) {
                    u64 key = part->name->Key();
                    fn_7101172764(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                    debris_shown = true;
                } else if (!NameHas(part->name, "Root")) {
                    u64 key = part->name->Key();
                    fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                }
            }
        }
    }
    if (UnkCollisionContainer* colliders = self->owner->colliders) {
        for (u32 i = 0; i < colliders->records.count; ++i) {
            auto* collider = reinterpret_cast<UnkItemLifeCollider_l8e0adc*>(
                fn_710086420c(reinterpret_cast<UnkPowerBombColliders_ovb0127*>(colliders), i));
            UnkStringData* const& name = collider->name;
            ++name->reads;
            bool enabled;
            if (name != &g_unk_7101d47108->empty && CRntString_Contains(&name->String(), "_DEBRIS"))
                enabled = true;
            else
                enabled = false;
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), enabled);
        }
    }
    // The entity stays while its debris shows or its object is still found.
    bool keep = debris_shown;
    if (fn_71005c1008(reinterpret_cast<u64>(self->owner)))
        keep = true;
    if (!keep) {
        fn_7100232c68(self->owner);
        if (char* ai = fn_71004dad10(reinterpret_cast<UnkActor*>(self->owner))) {
            if (self->field_0x338) {
                fn_71007c1ce0(ai, self->field_0x338);
                self->field_0x338 = nullptr;
            }
        }
        fn_7100874460(self->field_0x290, reinterpret_cast<UnkWeakTarget**>(&self->owner));
    }
    if (UnkStruct_710024fa0c* component = fn_71008e0e54(reinterpret_cast<UnkStruct_7100233684*>(self->owner)))
        fn_710024fa0c(component, false);
}
