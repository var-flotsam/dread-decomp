// CGrappleBeamComponent unit (tools/tu_map.py unit 1277): places the beam's effects between its start (the point
// fn_710081b920 answers) and its end (the owner's position plus the beam's length along its direction): the beam
// part is moved there, turned to the beam's angle and has the alpha of its "init", "core" and "edge" colours set to
// the length over fMaxLength; the start and end parts are moved to either end. Names and layouts are placeholders.
#include <math.h>

#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CFXComponent.h"
#include "game/CGrappleBeamComponent.h"
#include "game/UnkSlots.h"
#include "game/UnkTU133.h"
#include "game/UnkTU134.h"
#include "game/UnkTU253.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

// The answer of fn_710081b920 (src/game/CGrappleBeamComponent/CGrappleBeamComponent_b50.cpp), here read as
// a position.
struct UnkGrappleAnswer_b50 {
    void* field_0x0;
    u32 field_0x8;

    UnkGrappleAnswer_b50() : field_0x0(nullptr), field_0x8(0) {}
    UnkGrappleAnswer_b50(const UnkGrappleAnswer_b50& other);
};
struct UnkGrappleBeam_b50;
UnkGrappleAnswer_b50 fn_710081b920(UnkGrappleBeam_b50* self);

struct UnkGrappleTunable_l8194e8 {
    u8 pad_0x0[0x2c];
    f32 max_length;  // 0x2c, fMaxLength
};

// An effect part as this function reads it.
struct UnkGrapplePart_l8194e8 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x154];
    UnkSlots* slots;  // 0x160
};
static_assert(__builtin_offsetof(UnkGrapplePart_l8194e8, slots) == 0x160, "layout");

struct UnkGrappleOwner_l8194e8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0x228];
    UnkWorld* world;  // 0x280
};
static_assert(__builtin_offsetof(UnkGrappleOwner_l8194e8, world) == 0x280, "layout");

struct UnkGrappleBeam_l8194e8 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_l8194e8* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 dir_x;  // 0x40
    f32 dir_y;  // 0x44
    u8 pad_0x48[0x4f8];
    f32 length;  // 0x540
    u8 pad_0x544[0x1dc];
    CStrId end_part;    // 0x720
    CStrId start_part;  // 0x728
    CStrId beam_part;   // 0x730
};
static_assert(__builtin_offsetof(UnkGrappleBeam_l8194e8, length) == 0x540, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l8194e8, beam_part) == 0x730, "layout");

static inline u64 HashOf(const char* name) {
    return fn_7100001570(name, __builtin_strlen(name));
}

// The part of the world named `name`, or null when the world does not have it (fn_710114e680).
static inline UnkGrapplePart_l8194e8* SwitchPart(UnkWorld* world, const CStrId& name) {
    auto* part = reinterpret_cast<UnkGrapplePart_l8194e8*>(fn_7101152b90(world, name.data->Key()));
    return part && fn_710114e680(reinterpret_cast<UnkObject*>(world), part->kind) ? part : nullptr;
}

// The same, checked with fn_710114e668.
static inline UnkGrapplePart_l8194e8* FxPart(UnkWorld* world, const CStrId& name) {
    auto* part = reinterpret_cast<UnkGrapplePart_l8194e8*>(fn_7101152b90(world, name.data->Key()));
    return part && fn_710114e668(reinterpret_cast<UnkObject*>(world), part->kind) ? part : nullptr;
}

// Sets the alpha of the colour named `name`.
static inline void SetAlpha(UnkGrapplePart_l8194e8* part, const char* name, f32 alpha) {
    u64 key = HashOf(name);
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_71001c2bac(part->slots, key, 1, &color, 1);
    color.a = alpha;
    fn_71001c28f8(part->slots, key, 1, &color, 1);
}

void fn_71008194e8(UnkGrappleBeam_l8194e8* self) {
    UnkGrappleAnswer_b50 answer = fn_710081b920(reinterpret_cast<UnkGrappleBeam_b50*>(self));
    const UnkVec3f& start = reinterpret_cast<const UnkVec3f&>(answer);
    f32 end_x = self->owner->x + self->length * self->dir_x;
    f32 end_y = self->owner->y + self->length * self->dir_y;
    f32 run = end_x - start.x;
    f32 rise = end_y - start.y;
    UnkVec3f end(start);
    end.x = end_x;
    end.y = end_y;
    UnkVec3f angles(0.0f, 0.0f, atan2f(rise, run));
    f32 dx = start.x - end.x;
    f32 dy = start.y - end.y;
    f32 dz = start.z - end.z;
    f32 scale = sqrtf(dx * dx + dy * dy + dz * dz) /
                reinterpret_cast<UnkGrappleTunable_l8194e8*>(&g_unk_71727232d8)->max_length;

    UnkGrapplePart_l8194e8* beam = SwitchPart(self->owner->world, self->beam_part);
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(beam), &start);
    fn_7100145f3c(reinterpret_cast<UnkSwitchObject*>(beam), &angles);
    SetAlpha(beam, "init", scale);
    SetAlpha(beam, "core", scale);
    SetAlpha(beam, "edge", scale);

    UnkGrapplePart_l8194e8* first = FxPart(self->owner->world, self->end_part);
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(first), &start);
    fn_7100146d98(reinterpret_cast<UnkFxObject_ovb0041*>(first), &angles);
    UnkGrapplePart_l8194e8* last = FxPart(self->owner->world, self->start_part);
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(last), &end);
    fn_7100146d98(reinterpret_cast<UnkFxObject_ovb0041*>(last), &angles);
}
