// CDemolitionBlockLifeComponent unit (tools/tu_map.py unit 1374): restores the intact block: hides the model
// parts and turns off the colliders whose names contain "_DEBRIS" (shows and turns on the rest), finds the
// model's tracked node and resets it, then notifies the blackboard. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CBlackboard.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU218.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStringData;
using base::math::CVector4D;

struct UnkPowerBombCollider_ovb0127;
struct UnkPowerBombColliders_ovb0127;
UnkPowerBombCollider_ovb0127* fn_710086420c(UnkPowerBombColliders_ovb0127*, u32);

struct UnkDemolitionPart {
    UnkStringData* name;
};

struct UnkDemolitionNodeTarget {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkDemolitionNode {
    u8 pad_0x0[0x240];
    CVector4D value;  // 0x240
    u8 pad_0x250[0x2e0 - 0x250];
    UnkDemolitionNodeTarget* target;  // 0x2e0
};

struct UnkDemolitionSlots {
    u8 pad_0x0[0x48];
    UnkDemolitionNode** nodes;  // 0x48
};

struct UnkDemolitionModelResource {
    u8 pad_0x0[0x78];
    UnkDemolitionSlots* slots;  // 0x78
};

struct UnkPowerBombPartList_ovb0127;
struct UnkPowerBombPart_ovb0127;
u32 fn_71001c1440(UnkPowerBombPartList_ovb0127* list);
UnkPowerBombPart_ovb0127* fn_71001c1450(UnkPowerBombPartList_ovb0127* list, u32 index);

// The model's named parts (fn_71001c1440 counts them, fn_71001c1450 returns one).
static inline u32 PartCount(UnkDemolitionModelResource* resource) {
    return fn_71001c1440(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource));
}
static inline UnkDemolitionPart* PartAt(UnkDemolitionModelResource* resource, u32 index) {
    return reinterpret_cast<UnkDemolitionPart*>(
        fn_71001c1450(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource), index));
}

struct UnkDemolitionModelData {
    u8 pad_0x0[0x138];
    UnkDemolitionModelResource* resource;  // 0x138

    UnkDemolitionNode* Node(u32 index) const { return resource->slots->nodes[index]; }
};

struct UnkDemolitionModel {
    u8 pad_0x0[0x50];
    UnkDemolitionModelData* data;  // 0x50
};

struct UnkDemolitionCollider {
    u8 pad_0x0[0x20];
    UnkStringData* name;  // 0x20
};

struct UnkDemolitionOwner {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkDemolitionBlockLife {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkDemolitionOwner* owner;  // 0x30
    u8 pad_0x38[0x360 - 0x38];
    UnkDemolitionNode* node;  // 0x360

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkDemolitionBlockLife*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};

static inline UnkDemolitionModel* ModelOf(UnkDemolitionOwner* owner) {
    return reinterpret_cast<UnkDemolitionModel*>(fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(owner)));
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&name->String(), str);
}

void fn_71008c6744(UnkDemolitionBlockLife* self) {
    if (UnkDemolitionModel* model = ModelOf(self->owner)) {
        if (UnkDemolitionModelData* data = model->data) {
            for (u32 i = 0; i < PartCount(data->resource); ++i) {
                UnkDemolitionPart* part = PartAt(data->resource, i);
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
            auto* collider = reinterpret_cast<UnkDemolitionCollider*>(fn_710086420c(reinterpret_cast<UnkPowerBombColliders_ovb0127*>(colliders), i));
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
    UnkDemolitionModelData* data = nullptr;
    if (UnkDemolitionOwner* owner = self->owner) {
        if (UnkDemolitionModel* model = ModelOf(owner))
            data = model->data;
    }
    u32 index = 0;
    u64 key = 0x8bbd1bb96cba5b00;
    if (fn_7100199d88(data->resource->slots, &key, &index)) {
        UnkDemolitionNode* node = data ? data->Node(index) : nullptr;
        self->node = node;
        if (node) {
            f32 x = node->value.x;
            f32 z = node->value.z;
            f32 w = node->value.w;
            CVector4D value = {x, 0.0f, z, w};
            fn_710007b518(&node->value, &value);
            if (UnkDemolitionNodeTarget* target = node->target)
                *target->value = value;
        }
    }
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
}
