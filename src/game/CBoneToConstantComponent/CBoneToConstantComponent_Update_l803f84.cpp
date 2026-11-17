// CBoneToConstantComponent unit (tools/tu_map.py unit 1264): reads the named bone's transform from the owner's
// model and writes its translation into one constant slot of the selected node (or of every node).
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU218.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::math::CVector4D;

struct UnkBoneConstantTarget {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkBoneConstantNode {
    u8 pad_0x0[0x14c];
    CVector4D constantsA[10];                // 0x14c
    u8 pad_0x1ec[4];
    UnkBoneConstantTarget* targetsA[10];     // 0x1f0
    CVector4D constantsB[10];                // 0x240
    UnkBoneConstantTarget* targetsB[10];     // 0x2e0
};
static_assert(__builtin_offsetof(UnkBoneConstantNode, targetsB) == 0x2e0, "UnkBoneConstantNode layout");

struct UnkBoneConstantSlots {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
    u8 pad_0x30[0x18];
    UnkBoneConstantNode** nodes;  // 0x48
};

struct UnkBoneConstantResource {
    u8 pad_0x0[0x78];
    UnkBoneConstantSlots* slots;  // 0x78
};

struct UnkBoneConstantModelData {
    u8 pad_0x0[0x138];
    UnkBoneConstantResource* resource;  // 0x138

    UnkBoneConstantNode* Node(u32 index) const { return resource->slots->nodes[index]; }
};

struct UnkBoneConstantModel {
    u8 pad_0x0[0x50];
    UnkBoneConstantModelData* data;  // 0x50
};

struct UnkBoneConstantOwner {
    u8 pad_0x0[0x228];
    UnkBoneConstantModel* model;  // 0x228
};

struct UnkBoneToConstant {
    u8 pad_0x0[0x30];
    UnkBoneConstantOwner* owner;  // 0x30
    u8 pad_0x38[0x10];
    u64 bone;      // 0x48
    u64 nodeKey;   // 0x50, ~0 for every node
    u32 slot;      // 0x58
    u32 useA;      // 0x5c
};

static inline void SetConstant(UnkBoneConstantNode* node, const UnkBoneToConstant* self, const CVector4D& value) {
    u32 slot = self->slot;
    if (self->useA == 1) {
        Assign(node->constantsA[slot], value);
        if (UnkBoneConstantTarget* target = node->targetsA[slot])
            *target->value = value;
    } else {
        Assign(node->constantsB[slot], value);
        if (UnkBoneConstantTarget* target = node->targetsB[slot])
            *target->value = value;
    }
}

void fn_7100803f84(UnkBoneToConstant* self) {
    UnkBoneConstantModel* model = self->owner->model;
    if (!model)
        return;
    UnkBoneConstantModelData* data = model->data;
    if (!data)
        return;
    const u64* bone = &self->bone;
    f32 m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1;
    if (!fn_71001c1694(reinterpret_cast<char*>(data->resource), bone, m))
        return;
    base::math::UnkVector4D translation(m[3], m[7], m[11], 1.0f);
    if (self->nodeKey == ~0ull) {
        UnkBoneConstantSlots* slots = data->resource->slots;
        u32 count = slots->count;
        for (u32 i = 0; i < count; ++i)
            SetConstant(data->Node(i), self, translation);
    } else {
        u32 index = 0;
        if (!fn_7100199d88(data->resource->slots, &self->nodeKey, &index))
            return;
        SetConstant(data->Node(index), self, translation);
    }
}
