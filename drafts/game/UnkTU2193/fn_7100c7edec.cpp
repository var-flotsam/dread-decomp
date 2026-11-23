// Unit 2193: writes a colour, scaled by its intensity, into one constant slot of the selected model
// node (or of every node), in the slot set the target's kind names. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;
using base::math::UnkVector4D;

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

struct UnkTU2193Light {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
    f32 intensity;  // 0x10
};

struct UnkTU2193Target {
    u64 node;         // 0x0, ~0 for every node
    void* field_0x8;  // what picks the slot (fn_7100c7b648)
    u64 kind;         // 0x10, a name's hash
};

struct UnkTU2193 {
    UnkTU2193Light* light;  // 0x0
    u8 pad_0x8[0x8];
    UnkTU2193Target* target;  // 0x10
    u8 pad_0x18[0x18];
    UnkBoneConstantOwner* owner;  // 0x30
    u32 field_0x38;
};

u32 fn_7100c7b648(void* target);
[[noreturn]] void fn_7100c7f0a0();

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// The slot set a kind's name selects: 1 the first, 0 the second, 2 geometry (not written here).
static inline s32 KindOf(u64 name) {
    if (name == 0xbd2673e0bce97b56ull)
        return 1;
    if (name == 0x61839512038ac025ull)
        return 0;
    return StringCrc64("E_GEOMETRY", 10) == name ? 2 : 0x7fffffff;
}

void fn_7100c7edec(UnkTU2193* self) {
    if (self->field_0x38)
        fn_7100c7f0a0();
    UnkBoneConstantModel* model = self->owner->model;
    if (!model)
        return;
    UnkBoneConstantModelData* data = model->data;
    if (!data)
        return;
    const UnkTU2193Light* light = self->light;
    CColor4F color(NonNegative(light->r * light->intensity), NonNegative(light->intensity * light->g),
                   NonNegative(light->intensity * light->b), NonNegative(light->a));
    u32 slot = fn_7100c7b648(self->target->field_0x8);
    s32 kind = KindOf(self->target->kind);
    if (self->target->node == ~0ull) {
        for (u32 i = 0; i < data->resource->slots->count; ++i) {
            UnkBoneConstantNode* node = data->Node(i);
            if (kind == 0) {
                UnkVector4D value = fn_7100030878(&color);
                Assign(node->constantsB[slot], value);
                if (UnkBoneConstantTarget* target = node->targetsB[slot])
                    *target->value = value;
            } else if (kind == 1) {
                UnkVector4D value = fn_7100030878(&color);
                Assign(node->constantsA[slot], value);
                if (UnkBoneConstantTarget* target = node->targetsA[slot])
                    *target->value = value;
            }
        }
    } else {
        u32 index = ~0u;
        if (!fn_7100199d88(data->resource->slots, &self->target->node, &index))
            return;
        UnkBoneConstantNode* node = data->Node(index);
        if (kind == 0) {
            UnkVector4D value = fn_7100030878(&color);
            Assign(node->constantsB[slot], value);
            if (UnkBoneConstantTarget* target = node->targetsB[slot])
                *target->value = value;
        } else if (kind == 1) {
            UnkVector4D value = fn_7100030878(&color);
            Assign(node->constantsA[slot], value);
            if (UnkBoneConstantTarget* target = node->targetsA[slot])
                *target->value = value;
        }
    }
}
