// CItemLifeComponent unit (tools/tu_map.py unit 1379): the item dies: shows the model's "_DEBRIS" parts and
// hides the others (but "Root"), turns on only the debris colliders, takes the entity out unless debris shows
// or its object is still found, stores "Dead" in the scenario's blackboard section and notifies the unit's
// listeners. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CCoreXAIComponent.h"
#include "game/CDropComponent.h"
#include "game/CInputComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU253.h"
#include "game/UnkTU341.h"
#include "game/UnkTU883.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPowerBombCollider_ovb0127;
struct UnkPowerBombColliders_ovb0127;
struct UnkPowerBombPartList_ovb0127;
struct UnkPowerBombPart_ovb0127;
struct UnkHitInfo_A5_b63;
struct UnkActor;
struct UnkWeakTarget;
struct UnkItemLifeOwner_l8e1ea4;
struct UnkItemLife_l8e1ea4;

// Three vectors and a flag; element types are unknown.
struct UnkItemLifeLists {
    base::global::CRntVector<void*> lists[3];
    bool field_0x78;

    UnkItemLifeLists() : lists{base::global::CRntVector<void*>(0xf), base::global::CRntVector<void*>(0xf), base::global::CRntVector<void*>(0xf)} {
        field_0x78 = false;
    }
};

struct UnkClone_fn_710082d0ac;
using UnkItemLifeListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkItemLifeListeners*, const u64*);

struct UnkItemLifeComponent_b43a2;
void fn_71008e21fc(UnkItemLifeComponent_b43a2* self);

struct UnkItemLifePart_l8e1ea4 {
    UnkStringData* name;
};

struct UnkItemLifeModelResource_l8e1ea4;

// The model's named parts (fn_71001c1440 counts them, fn_71001c1450 returns one).
static inline u32 PartCount(UnkItemLifeModelResource_l8e1ea4* resource) {
    return fn_71001c1440(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource));
}
static inline UnkItemLifePart_l8e1ea4* PartAt(UnkItemLifeModelResource_l8e1ea4* resource, u32 index) {
    return reinterpret_cast<UnkItemLifePart_l8e1ea4*>(
        fn_71001c1450(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource), index));
}

struct UnkItemLifeModelData_l8e1ea4 {
    u8 pad_0x0[0x138];
    UnkItemLifeModelResource_l8e1ea4* resource;  // 0x138
};

struct UnkItemLifeModel_l8e1ea4 {
    u8 pad_0x0[0x50];
    UnkItemLifeModelData_l8e1ea4* data;  // 0x50
};

struct UnkItemLifeCollider_l8e1ea4 {
    u8 pad_0x0[0x20];
    UnkStringData* name;  // 0x20
};

struct UnkItemLifeOwner_l8e1ea4 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkItemLifeValue_l8e1ea4 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkItemLife_l8e1ea4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkItemLifeOwner_l8e1ea4* owner;  // 0x30
    u8 pad_0x38[0x268 - 0x38];
    UnkItemLifeValue_l8e1ea4* values;  // 0x268
    u64* keys;                         // 0x270
    u32 count;                         // 0x278
    u8 pad_0x27c[0x290 - 0x27c];
    u8 field_0x290[0xa8];
    UnkHitInfo_A5_b63* field_0x338;  // 0x338
};

static inline UnkItemLifeModel_l8e1ea4* ModelOf(UnkItemLifeOwner_l8e1ea4* owner) {
    return reinterpret_cast<UnkItemLifeModel_l8e1ea4*>(
        fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(owner)));
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&name->String(), str);
}

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

void fn_71008e1ea4(UnkItemLife_l8e1ea4* self) {
    bool debris_shown = false;
    if (UnkItemLifeModel_l8e1ea4* model = ModelOf(self->owner)) {
        if (UnkItemLifeModelData_l8e1ea4* data = model->data) {
            for (u32 i = 0; i < PartCount(data->resource); ++i) {
                UnkItemLifePart_l8e1ea4* part = PartAt(data->resource, i);
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
            auto* collider = reinterpret_cast<UnkItemLifeCollider_l8e1ea4*>(
                fn_710086420c(reinterpret_cast<UnkPowerBombColliders_ovb0127*>(colliders), i));
            UnkStringData* const& name = collider->name;
            ++name->reads;
            bool enabled;
            if (name != &g_unk_7101d47108->empty && fn_710007fb9c(&name->String(), "_DEBRIS"))
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
    fn_71008e21fc(reinterpret_cast<UnkItemLifeComponent_b43a2*>(self));
    if (UnkStruct_710024fa0c* component = fn_71008e0e54(reinterpret_cast<UnkStruct_7100233684*>(self->owner)))
        fn_710024fa0c(component, false);
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 n = self->count;
        if (n) {
            u64* k = self->keys;
            u64 key = (u64)g_unk_7172732320.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        UnkItemLifeValue_l8e1ea4* v = self->values;
        bool dead = true;
        SetSectionBool(blackboard, section_id, &v[index].name, &dead);
    }
    u64 arg = reinterpret_cast<u64>(self);
    base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
        reinterpret_cast<UnkItemLifeListeners*>(&g_unk_7172732378), &arg);
}
