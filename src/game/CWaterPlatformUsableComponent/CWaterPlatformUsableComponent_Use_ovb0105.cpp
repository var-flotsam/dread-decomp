// CWaterPlatformUsableComponent's unit (tools/tu_map.py): the platform's set-up after the base step (the
// body's position and height, the area's bounds, from the owner's collision objects) and the end of a use
// (the level eases to a half, the script hears "OnUseCompleted", the guide light is set and every E.M.M.I.
// is told). Names and layouts are placeholders.
#include "base/UnkTimedCurve.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CActorComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkSmBase_b60;

// The platform's two collision object names (the second right after the first).
extern CStrId g_unk_71727537e8[2] __attribute__((visibility("hidden")));

struct UnkWaterPlatformXform_b0105 {
    u8 pad_0x0[0xc];
    f32 x;  // 0xc
    u8 pad_0x10[0xc];
    f32 y;  // 0x1c
    u8 pad_0x20[0xc];
    f32 z;  // 0x2c
};

struct UnkWaterPlatformShape_b0105 {
    void** vtable;
};

// The collision objects as this unit uses them: the transform (vf_0x70), the shape at +0x158 with its size
// (vf_0x68), and an update (vf_0x98) pending while the flag at +0x60 is set, and the bounds at +0x40.
struct UnkWaterPlatformCollider_b0105 {
    void** vtable;
    u8 pad_0x8[0x38];
    u32 bounds[4];  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0xf7];
    UnkWaterPlatformShape_b0105* shape;  // 0x158
};

struct UnkWaterPlatformOwner_b0105 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkWaterPlatformUsable_b0105 {
    u8 pad_0x0[0x30];
    UnkWaterPlatformOwner_b0105* owner;  // 0x30
    u8 pad_0x38[0x504];
    UnkVec3f position;  // 0x53c
    f32 height;  // 0x548
    u8 pad_0x54c[0xc];
    u32 field_0x558;
    u8 pad_0x55c[0x60];
    u32 field_0x5bc;
    u8 pad_0x5c0[0x48];
    u32 bounds[4];  // 0x608
};
static_assert(__builtin_offsetof(UnkWaterPlatformUsable_b0105, bounds) == 0x608, "layout");

void fn_7100aa54c0(UnkWaterPlatformUsable_b0105* self) {
    fn_710024facc(reinterpret_cast<UnkSmBase_b60*>(self));
    auto* body = reinterpret_cast<UnkWaterPlatformCollider_b0105*>(
        fn_71008640a4(self->owner->collision, &g_unk_71727537e8[0]));
    auto* xform =
        reinterpret_cast<UnkWaterPlatformXform_b0105* (*)(UnkWaterPlatformCollider_b0105*)>(body->vtable[0x70 / 8])(body);
    self->position = UnkVec3f(xform->x, xform->y, xform->z);
    UnkWaterPlatformShape_b0105* shape = body->shape;
    UnkVec3f size = reinterpret_cast<UnkVec3f (*)(UnkWaterPlatformShape_b0105*)>(shape->vtable[0x68 / 8])(shape);
    self->height = size.y;
    self->field_0x5bc = self->field_0x558;
    auto* area = reinterpret_cast<UnkWaterPlatformCollider_b0105*>(
        fn_71008640a4(self->owner->collision, &g_unk_71727537e8[1]));
    if (area) {
        if (area->dirty)
            reinterpret_cast<void (*)(UnkWaterPlatformCollider_b0105*)>(area->vtable[0x98 / 8])(area);
        self->bounds[0] = area->bounds[0];
        self->bounds[1] = area->bounds[1];
        self->bounds[2] = area->bounds[2];
        self->bounds[3] = area->bounds[3];
    }
}

struct UnkEmmyRegistry_b55;
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
struct UnkScriptCaller;

struct UnkEmmyEntry_b0105 {
    u8 pad_0x0[0x10];
    void* emmy;
};

#pragma pack(push, 4)
struct UnkEmmyRegistry_b0105 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkEmmyEntry_b0105* items[1];  // 0xc, 4-byte aligned
};
#pragma pack(pop)

struct UnkWaterPlatformOwner2_b0105 {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
};

struct UnkWaterPlatform2_b0105 {
    u8 pad_0x0[0x30];
    UnkWaterPlatformOwner2_b0105* owner;  // 0x30
    u8 pad_0x38[0x52c];
    f32 level;  // 0x564
    UnkTimedCurve curve;  // 0x568
    u8 pad_0x5b8[0x4];
    f32 field_0x5bc;
    u8 pad_0x5c0[5];
    bool used;  // 0x5c5
    u8 pad_0x5c6[2];
    u8 block[1];  // 0x5c8
};
static_assert(__builtin_offsetof(UnkWaterPlatform2_b0105, block) == 0x5c8, "layout");

void fn_7100aa6a2c(UnkWaterPlatform2_b0105* self, f32 t);

static inline void Ease(UnkTimedCurve* curve, f32 start, f32 target, f32 duration,
                        const base::global::CRntString& modifiers) {
    fn_7100075f78(start, target, duration, curve, &modifiers);
}

// The use has completed: the level eases (from where it is) towards a half over a second, the script
// hears "OnUseCompleted", the guide light colour is set and every E.M.M.I. is told (fn_710110fae4).
void fn_7100aa675c(UnkWaterPlatform2_b0105* self) {
    self->field_0x5bc = 0.0f;
    self->used = true;
    fn_7100aa0034(self);
    Ease(&self->curve, self->level, 0.5f, 1.0f, base::global::CRntString(g_unk_710162e2ed));
    fn_7100aa6a2c(self, 1.0f);
    if (UnkScriptCaller* script = self->owner->script)
        fn_7100305088(script, "OnUseCompleted", g_unk_710162e2ed);
    u64 hash = fn_7100001570("GUIDE_LGT_00", 12);
    UnkRenderManager* render = g_unk_7101d47300;
    base::math::CVector4D colour;
    colour.x = 15.0f;
    colour.y = 15.0f;
    colour.z = 15.0f;
    colour.w = 1.0f;
    fn_710021a0b4(render, &hash, &colour);
    UnkEmmyRegistry_b0105* registry = reinterpret_cast<UnkEmmyRegistry_b0105*>(g_unk_71726af0a8);
    u64 bytes = (u64)registry->count * sizeof(void*);
    asm("" : "+r"(bytes));
    for (UnkEmmyEntry_b0105 **it = registry->items, **end = (UnkEmmyEntry_b0105**)((char*)it + bytes); it != end; ++it)
        fn_710110fae4(*it ? (*it)->emmy : nullptr, self->block);
}
