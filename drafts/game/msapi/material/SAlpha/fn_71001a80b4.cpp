// Unit 229 (msapi::material::SAlpha and the unit's other material types): applies a pass's texture
// stages to a render group: enables each stage, binds its texture (looked up by a 1-based name index
// into the material's name list at +0x3a8, grown with empty ids on demand) and sets its sampler state.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "engine/UnkGraphicsSystem.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkTU182.h"
#include "game/UnkTU211.h"
#include "types.h"

using base::global::CStrId;

void* fn_71001d086c(UnkGraphicsSystem* self, const base::global::CStrId* key);
void fn_7100160cf0(u32 index, u32 value, u32 slot);
void fn_7100160b30(u32 a, u32 color, u32 c);
void fn_7100160c90(u32 a, f32 value, u32 c);
void fn_7100160bd0(u32 a, f32 value, u32 c);
void fn_7100160e50(s32 a, s32 b, u32 c);
void fn_7100160c30(u32 index, u32 slot, f32 value);

struct UnkMatStateOwner_ovb0098;

struct UnkMatTextureStage_l1a80b4 {
    u8 pad_0x0[0x20];
    u32 slot;  // 0x20
    u8 pad_0x24[0xc];
    s32 field_0x30;
    s32 field_0x34;
    s32 field_0x38;
    u32 field_0x3c;
    s32 field_0x40;
    s32 field_0x44;
    u32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    u8 pad_0x5c[0x1c];
    u64 texture;  // 0x78: 1-based index into the owner's names, or the texture itself
    u8 pad_0x80[0x8];
};
static_assert(sizeof(UnkMatTextureStage_l1a80b4) == 0x88, "stage size");

struct UnkMatTextureStages_l1a80b4 {
    UnkMatTextureStage_l1a80b4* data;
    u32 count;
};

struct UnkMatTextureOwner_l1a80b4 {
    u8 pad_0x0[0x3a8];
    base::UnkStrIdVec names;  // 0x3a8
};

struct UnkDecalLimits_l1a80b4 {
    u8 pad_0x0[0xac];
    f32 max_scale;  // 0xac
};

// The name at `index`, growing the list with empty names when it may grow and is too short.
inline CStrId* NameAt(base::UnkStrIdVec* v, u32 index) {
    if (v->count <= index && v->field_0x14) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0)
            cap = (index / v->step + 1) * v->step;
        v->Reserve(cap);
        u32 n = index + 1;
        v->Reserve(n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkEmptyStrId();
        v->count = n;
    }
    return &v->data[index];
}

void fn_71001a80b4(UnkMatStateOwner_ovb0098* owner, u32 id, void* stageList) {
    auto* self = reinterpret_cast<UnkMatTextureOwner_l1a80b4*>(owner);
    auto* stages = static_cast<UnkMatTextureStages_l1a80b4*>(stageList);
    for (UnkMatTextureStage_l1a80b4* s = stages->data; s != stages->data + stages->count; ++s) {
        fn_71001606f0(s->slot, true, id);
        u64 texture = s->texture;
        if (texture != 0 && texture <= self->names.count) {
            UnkGraphicsSystem* gfx = reinterpret_cast<UnkGraphicsSystem*>(g_unk_710b372a98);
            u32 index = texture - 1;
            texture = reinterpret_cast<u64>(fn_71001d086c(gfx, NameAt(&self->names, index)));
        }
        SetGraphicsSlotPointer(s->slot, reinterpret_cast<void*>(texture), id);
        fn_7100160990(s->slot, s->field_0x30, id);
        fn_7100160ab0(s->slot, s->field_0x34, id);
        fn_7100160d50(s->slot, s->field_0x38, id);
        fn_7100160cf0(s->slot, s->field_0x3c, id);
        fn_7100160810(s->slot, s->field_0x40, id);
        fn_7100160890(s->slot, s->field_0x44, id);
        fn_7100160b30(s->slot, s->field_0x48, id);
        fn_7100160c90(s->slot, s->field_0x4c, id);
        fn_7100160bd0(s->slot, s->field_0x50, id);
        fn_7100160e50(s->slot, (s32)s->field_0x58, id);
        f32 max = reinterpret_cast<UnkDecalLimits_l1a80b4*>(g_unk_716e352450)->max_scale;
        f32 scale = s->field_0x54 >= max ? max : s->field_0x54;
        fn_7100160c30(s->slot, id, !(s->field_0x54 <= 1.0f) ? scale : 1.0f);
    }
}
