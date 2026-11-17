// CItemLifeComponent unit (tools/tu_map.py unit 1379): unless the component is a door's life component, hides
// the model parts and turns off the colliders whose names contain "_DEBRIS" (shows and turns on the rest);
// then caches the model's tracked node and notifies the blackboard. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU218.h"
#include "game/UnkTU253.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkPowerBombCollider_ovb0127;
struct UnkPowerBombColliders_ovb0127;
struct UnkPowerBombPartList_ovb0127;
struct UnkPowerBombPart_ovb0127;

struct UnkItemLifePart_l8e0f30 {
    UnkStringData* name;
};

struct UnkItemLifeNode_l8e0f30;

struct UnkItemLifeSlots_l8e0f30 {
    u8 pad_0x0[0x48];
    UnkItemLifeNode_l8e0f30** nodes;  // 0x48
};

struct UnkItemLifeModelResource_l8e0f30 {
    u8 pad_0x0[0x78];
    UnkItemLifeSlots_l8e0f30* slots;  // 0x78
};

// The model's named parts (fn_71001c1440 counts them, fn_71001c1450 returns one).
static inline u32 PartCount(UnkItemLifeModelResource_l8e0f30* resource) {
    return fn_71001c1440(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource));
}
static inline UnkItemLifePart_l8e0f30* PartAt(UnkItemLifeModelResource_l8e0f30* resource, u32 index) {
    return reinterpret_cast<UnkItemLifePart_l8e0f30*>(
        fn_71001c1450(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource), index));
}

struct UnkItemLifeModelData_l8e0f30 {
    u8 pad_0x0[0x138];
    UnkItemLifeModelResource_l8e0f30* resource;  // 0x138

    UnkItemLifeNode_l8e0f30* Node(u32 index) const { return resource->slots->nodes[index]; }
};

struct UnkItemLifeModel_l8e0f30 {
    u8 pad_0x0[0x50];
    UnkItemLifeModelData_l8e0f30* data;  // 0x50
};

struct UnkItemLifeCollider_l8e0f30 {
    u8 pad_0x0[0x20];
    UnkStringData* name;  // 0x20
};

struct UnkItemLifeOwner_l8e0f30 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkItemLife_l8e0f30 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkItemLifeOwner_l8e0f30* owner;  // 0x30
    u8 pad_0x38[0x348 - 0x38];
    UnkItemLifeNode_l8e0f30* node;  // 0x348

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkItemLife_l8e0f30*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};

static inline UnkItemLifeModel_l8e0f30* ModelOf(UnkItemLifeOwner_l8e0f30* owner) {
    return reinterpret_cast<UnkItemLifeModel_l8e0f30*>(
        fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(owner)));
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&name->String(), str);
}

static inline bool IsDoorLife(UnkItemLife_l8e0f30* self) {
    auto* obj = reinterpret_cast<base::reflection::UnkReflObject*>(self);
    return fn_710009d50c(obj->vf_0x0(), fn_71008cb078());
}

void fn_71008e0f30(UnkItemLife_l8e0f30* self) {
    if (!self || !IsDoorLife(self)) {
        if (UnkItemLifeModel_l8e0f30* model = ModelOf(self->owner)) {
            if (UnkItemLifeModelData_l8e0f30* data = model->data) {
                for (u32 i = 0; i < PartCount(data->resource); ++i) {
                    UnkItemLifePart_l8e0f30* part = PartAt(data->resource, i);
                    if (NameHas(part->name, "_DEBRIS")) {
                        u64 key = part->name->Key();
                        fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                    } else {
                        u64 key = part->name->Key();
                        fn_7101172764(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                    }
                }
            }
        }
        if (UnkCollisionContainer* colliders = self->owner->colliders) {
            for (u32 i = 0; i < colliders->records.count; ++i) {
                auto* collider = reinterpret_cast<UnkItemLifeCollider_l8e0f30*>(
                    fn_710086420c(reinterpret_cast<UnkPowerBombColliders_ovb0127*>(colliders), i));
                UnkStringData* const& name = collider->name;
                ++name->reads;
                bool enabled;
                if (name != &g_unk_7101d47108->empty && fn_710007fb9c(&name->String(), "_DEBRIS"))
                    enabled = false;
                else
                    enabled = true;
                fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), enabled);
            }
        }
    }
    if (UnkItemLifeOwner_l8e0f30* owner = self->owner) {
        if (UnkItemLifeModel_l8e0f30* model = ModelOf(owner)) {
            if (UnkItemLifeModelData_l8e0f30* data = model->data) {
                u32 index = 0;
                u64 key = 0x8bbd1bb96cba5b00;
                if (fn_7100199d88(data->resource->slots, &key, &index))
                    self->node = data->Node(index);
            }
        }
    }
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
}
