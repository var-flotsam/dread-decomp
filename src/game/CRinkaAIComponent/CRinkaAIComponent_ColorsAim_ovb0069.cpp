// CRinkaAIComponent's unit (tools/tu_map.py). fn_710065de1c applies the Rinka's four colours (constants at
// g_unk_710164a454 onwards, clamped by CColor4F's assignment): two to the slots (+0x138) of the model at
// +0x50 of the owner's +0x228, and one each to the slots (+0x160) of the owner's (+0x280) two parts found
// by hash (fn_7101152b90, checked by fn_710114e680). fn_710065e31c aims the Rinka at its target
// (fn_7100d7c990): the unit direction from the owner to the target is stored in the owner's movement
// (fn_710065e520, +0x6f0), the owner is turned to face it (its transform's z angle, the scale derived
// first when only the matrix is valid), +0x3c0 is set, and the timeline (+0x2e8) gets the type id under
// g_unk_710164a4a0. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSlots.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;

struct UnkComponent;
struct UnkObject;
UnkComponent* fn_7101152b90(UnkObject* object, u64 hash);

extern const CColor4F g_unk_710164a454 __attribute__((visibility("hidden")));
extern const CColor4F g_unk_710164a464 __attribute__((visibility("hidden")));
extern const CColor4F g_unk_710164a474 __attribute__((visibility("hidden")));
extern const CColor4F g_unk_710164a484 __attribute__((visibility("hidden")));

struct UnkRinkaModel_ovb0069 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};
struct UnkRinkaRender_ovb0069 {
    u8 pad_0x0[0x50];
    UnkRinkaModel_ovb0069* model;  // 0x50
};
struct UnkRinkaPart_ovb0069 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x154];
    UnkSlots* slots;  // 0x160
};
struct UnkRinkaColorOwner_ovb0069 {
    u8 pad_0x0[0x228];
    UnkRinkaRender_ovb0069* render;  // 0x228
    u8 pad_0x230[0x50];
    UnkObject* parts;  // 0x280
};
struct UnkRinkaColorAI_ovb0069 {
    u8 pad_0x0[0x30];
    UnkRinkaColorOwner_ovb0069* owner;  // 0x30
};

static inline UnkRinkaPart_ovb0069* Part(UnkObject* parts, u64 hash) {
    auto* part = reinterpret_cast<UnkRinkaPart_ovb0069*>(fn_7101152b90(parts, hash));
    return fn_710114e680(parts, part->kind) ? part : nullptr;
}

static inline const base::math::CVector4D* Vec(const CColor4F& c) {
    return reinterpret_cast<const base::math::CVector4D*>(&c);
}

void fn_710065de1c(UnkRinkaColorAI_ovb0069* self) {
    CColor4F body(1.0f, 1.0f, 1.0f, 1.0f);
    CColor4F glow(1.0f, 1.0f, 1.0f, 1.0f);
    CColor4F part_a(1.0f, 1.0f, 1.0f, 1.0f);
    CColor4F part_b(1.0f, 1.0f, 1.0f, 1.0f);
    fn_7100030728(&body, &g_unk_710164a454);
    fn_7100030728(&glow, &g_unk_710164a464);
    fn_7100030728(&part_a, &g_unk_710164a474);
    fn_7100030728(&part_b, &g_unk_710164a484);
    if (self->owner->render->model) {
        fn_71001c2764(self->owner->render->model->slots, 0, 0, Vec(body), 1, true);
        fn_71001c2764(self->owner->render->model->slots, 0, 1, Vec(glow), 1, true);
    }
    if (UnkObject* parts = self->owner->parts) {
        fn_71001c2764(Part(parts, 0x401ff13a7d0fb0ffull)->slots, 0, 0, Vec(part_a), 1, true);
        fn_71001c2764(Part(self->owner->parts, 0xd04bdcbf0c7f5b57ull)->slots, 0, 0, Vec(part_b), 1, true);
    }
}

extern "C" float atan2f(float y, float x);

extern const u64 g_unk_710164a4a0;

struct UnkRinkaMovement_ovb0069 {
    u8 pad_0x0[0x6f0];
    f32 dir_x;  // 0x6f0
    f32 dir_y;
    f32 dir_z;
};
UnkRinkaMovement_ovb0069* fn_710065e520(UnkActor* actor);

struct UnkRinkaOwner_ovb0069 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x254];
    UnkTimelineComponent* timeline;  // 0x2e8
    u8 pad_0x2f0[0xd0];
    bool aimed;  // 0x3c0
};
static_assert(__builtin_offsetof(UnkRinkaOwner_ovb0069, aimed) == 0x3c0, "layout");

struct UnkRinkaAimAI_ovb0069 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

void fn_710065e31c(UnkRinkaAimAI_ovb0069* self) {
    if (!fn_7100d7c990(self->owner))
        return;
    UnkActor* target = fn_7100d7c990(self->owner);
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44;
    f32 oy = owner->field_0x54;
    f32 oz = owner->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    UnkRinkaMovement_ovb0069* movement = fn_710065e520(self->owner);
    movement->dir_x = dx;
    movement->dir_y = dy;
    movement->dir_z = dz;
    f32 angle = atan2f(dy, dx);
    auto* o = reinterpret_cast<UnkRinkaOwner_ovb0069*>(self->owner);
    UnkXform& xf = o->xform;
    if ((xf.flags & 0xc) == 8) {
        xf.scale = fn_7100076cec(&xf);
        xf.flags |= 4;
    }
    xf.flags = (xf.flags & ~8u) | 2;
    xf.angle.x = 0.0f;
    xf.angle.y = 0.0f;
    xf.angle.z = angle;
    // A byte store: the original reloads the owner after it.
    *reinterpret_cast<u8*>(&o->aimed) = 1;
    auto* owner2 = reinterpret_cast<UnkRinkaOwner_ovb0069*>(self->owner);
    if (UnkTimelineComponent* timeline = owner2->timeline) {
        u32 id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7100a2cb7c(timeline, &g_unk_710164a4a0, &id, false, -1.0f);
    }
}
