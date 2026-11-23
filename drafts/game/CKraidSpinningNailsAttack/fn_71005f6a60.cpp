// CKraidSpinningNailsAttack's unit (tools/tu_map.py unit 918): throws the current nail. Loads the nail actor
// definition, places it at the launch point the entry's kind names on Kraid's model, aims it along the
// entry's angle (mirrored to Kraid's facing) and gives its movement the speed and gravity that make its
// arc reach the player (or a point by Kraid in his second phase); then plays the kind's throw on Kraid's
// animation. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

// The nail actor definition.
extern CStrId g_unk_71726f3180 __attribute__((visibility("hidden")));
// The distances the speed is interpolated between in Kraid's first phase.
extern f32 g_unk_7101a540f8;
extern f32 g_unk_7101a540fc;

// By the throw's kind (1 to 4): the action played on Kraid's animation, the launch point's name on his
// model, and the animation track.
extern const u64 g_unk_7101647d08[4] __attribute__((visibility("hidden")));
extern const u64 g_unk_7101647e08[4] __attribute__((visibility("hidden")));
extern const u64 g_unk_7101647d48[4] __attribute__((visibility("hidden")));

struct UnkKraidName_l5f6a60 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkKraidDefType_l5f6a60 {
    u32 id;
};

struct UnkKraidAI_l5f6a60 : UnkReflObject {
    u8 pad_0x8[0x1140 - 0x8];
    s32 phase;  // 0x1140
};

struct UnkKraidOwner_l5f6a60 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // +0x38
    u8 pad_0x94[0x220 - 0x94];
    void* animation;   // +0x220
    UnkEntity* model;  // +0x228
    u8 pad_0x230[0x10];
    UnkReflObject* ai;  // +0x240
};
static_assert(__builtin_offsetof(UnkKraidOwner_l5f6a60, ai) == 0x240, "layout");

// One throw of a group: which hand (1 to 4) and the angle.
struct UnkKraidNailEntry_l5f6a60 {
    u8 pad_0x0[0x8];
    s32 kind;  // +0x8
    u8 pad_0xc[0x4];
    f32 angle;  // +0x10, degrees
};

struct UnkKraidNail_l5f6a60 {
    void** vtable;

    void SetPosition(const UnkVec3f* position, bool flag) {
        reinterpret_cast<void (*)(UnkKraidNail_l5f6a60*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(
            this, position, flag);
    }
};

// The nail's character class: the speeds of a throw by kind (odd kinds first).
struct UnkKraidNailClass_l5f6a60 : UnkReflObject {
    u8 pad_0x8[0x138 - 0x8];
    f32 near_speed_odd;   // +0x138
    f32 far_speed_odd;    // +0x13c
    f32 near_speed_even;  // +0x140
    f32 far_speed_even;   // +0x144
    f32 speed_odd;        // +0x148
    f32 speed_even;       // +0x14c
};

struct UnkKraidNailMovement_l5f6a60 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkKraidNailClass_l5f6a60* nail_class;  // +0x10
    u8 pad_0x18[0x40 - 0x18];
    UnkVec3f direction;  // +0x40
    u8 pad_0x4c[0x108 - 0x4c];
    f32 gravity;  // +0x108
    u8 pad_0x10c[0x5d4 - 0x10c];
    f32 speed;  // +0x5d4
};
static_assert(__builtin_offsetof(UnkKraidNailMovement_l5f6a60, speed) == 0x5d4, "layout");

UnkKraidNailMovement_l5f6a60* fn_71005f6f7c(UnkKraidNail_l5f6a60* nail);
void fn_7100ac397c(UnkKraidNailMovement_l5f6a60* movement, UnkKraidOwner_l5f6a60* owner);
void fn_7100ac64cc(UnkKraidNailMovement_l5f6a60* movement);
UnkVec3f fn_71005fd8c0(UnkKraidAI_l5f6a60* kraid);

struct UnkKraidNailsAttack_l5f6a60 {
    u8 pad_0x0[0x8];
    UnkKraidOwner_l5f6a60* owner;  // +0x8
    u8 pad_0x10[0x48];
    UnkKraidNailEntry_l5f6a60* entry;  // +0x58
};

// Aims 100 units ahead of `point`.
static inline void AimAhead(UnkActorPosition* target, UnkVec3f point) {
    target->x = point.x + 100.0f;
    target->y = point.y;
    target->z = point.z;
}

static inline f32 Lerp(f32 from, f32 to, f32 t) {
    return from + t * (to - from);
}

void fn_71005f6a60(UnkKraidNailsAttack_l5f6a60* self) {
    u64 launch_point = ~0ull;
    UnkReflObject* ai = self->owner->ai;
    UnkKraidAI_l5f6a60* kraid =
        ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? static_cast<UnkKraidAI_l5f6a60*>(ai) : nullptr;

    u64 action;
    u64 track;
    s32 kind = self->entry->kind;
    if (kind >= 1 && kind <= 4) {
        action = g_unk_7101647d08[kind - 1];
        launch_point = g_unk_7101647e08[kind - 1];
        track = g_unk_7101647d48[kind - 1];
    } else {
        track = ~0ull;
        action = ~0ull;
    }

    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkKraidName_l5f6a60* name = reinterpret_cast<UnkKraidName_l5f6a60*>(g_unk_71726f3180.data);
    ++name->use_count;
    UnkKraidDefType_l5f6a60 type;
    type.id = g_unk_7172a0ba48;
    auto* nail = reinterpret_cast<UnkKraidNail_l5f6a60*>(fn_710119438c(
        factory, name->text, &g_unk_71726f3180, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!nail)
        return;

    UnkKraidOwner_l5f6a60* owner = self->owner;
    UnkVec3f position = owner->xform.GetTranslation();
    UnkMatrix44 point = {};
    point.m[15] = 1.0f;
    point.m[10] = 1.0f;
    point.m[5] = 1.0f;
    point.m[0] = 1.0f;
    if (fn_71011735e8(owner->model, &launch_point, &point, 0))
        position = UnkVec3f(point.m[3], point.m[7], point.m[11]);

    f32 angle = self->entry->angle * 3.141592653589793 / 180.0;
    UnkVec3f dir = fn_710007a390(UnkVec3f(1.0f, 0.0f, 0.0f), angle);
    if (self->owner->xform.GetMatrix().m[2] < 0.0f)
        dir.x = -dir.x;
    f32 inv = InvLengthEps(dir);
    dir.x *= inv;
    dir.y *= inv;
    dir.z *= inv;

    nail->SetPosition(&position, true);
    UnkKraidNailMovement_l5f6a60* movement = fn_71005f6f7c(nail);
    if (!movement)
        return;
    UnkKraidNailClass_l5f6a60* nail_class = movement->nail_class;
    if (!nail_class || !fn_710009d50c(nail_class->vf_0x0(), fn_7100602d44()))
        return;

    movement->direction = dir;
    fn_7100ac397c(movement, self->owner);
    UnkActorPosition target = fn_7100d7b7a8(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    if (kraid && kraid->phase == 1) {
        AimAhead(&target, fn_71005fd8c0(kraid));
    } else if (self->entry->kind == 2 || self->entry->kind == 4) {
        target.y += 150.0f;
    }
    f32 dy = target.y - position.y;
    f32 dx = target.x - position.x;
    f32 distance = fabsf(target.x - position.x);

    f32 speed;
    if (!kraid || kraid->phase != 0) {
        if (self->entry->kind == 1 || self->entry->kind == 3)
            speed = nail_class->speed_odd;
        else
            speed = nail_class->speed_even;
    } else {
        f32 t = distance / (g_unk_7101a540fc - g_unk_7101a540f8);
        if (self->entry->kind == 1 || self->entry->kind == 3)
            speed = Lerp(nail_class->near_speed_odd, nail_class->far_speed_odd, t);
        else
            speed = Lerp(nail_class->near_speed_even, nail_class->far_speed_even, t);
    }

    // The gravity of a parabola that leaves at `angle` with `speed` and passes through the target.
    dy -= distance * tanf(angle);
    f32 c = cosf(angle);
    movement->speed = speed;
    movement->gravity = dy * (c * (speed * (speed * -2.0f) * c)) / (dx * dx);
    fn_7100ac64cc(movement);

    base::reflection::refl_EmitU32(self->owner->animation, track, action, 2, &g_unk_710164b958,
                                   &g_unk_7172769740);
}
