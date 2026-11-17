// CKraidAcidBlobsAttack's unit: the attack's update. While active it advances its timers and, when the
// pattern's next entry is due, spawns an acid blob (big or small) and launches it on an arc that lands at
// the entry's distance; once the attack has run its length it tells the owner's animation. Names and
// layouts are placeholders.
#include <math.h>

#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAttack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::UnkReflObject;

struct UnkAcidBlobsPattern_b45a2;
struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;

// One entry of the launch pattern: how far out the blob lands (negative: at the target), whether it
// is a big one, and when it is due.
struct UnkAcidBlobsEntry_l5f131c {
    u8 pad_0x0[0x8];
    s32 distance;  // 0x8
    bool big;      // 0xc
    u8 pad_0xd[0x3];
    f32 time;  // 0x10
};

struct UnkAcidBlobsAnim_l5f131c {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkAcidBlobsDef_l5f131c {
    u8 pad_0x0[0xf0];
    UnkAcidBlobsAnim_l5f131c* end_anim;  // 0xf0
    f32 duration;                        // 0xf8
    f32 small_speed;                     // 0xfc
    f32 big_speed;                       // 0x100
    f32 near_angle;                      // 0x104
    f32 far_angle;                       // 0x108
};

struct UnkAcidBlobsAttack_l5f131c {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkAcidBlobsDef_l5f131c* def;  // 0x30
    bool active;                   // 0x38
    u8 pad_0x39[0x3];
    f32 timer;   // 0x3c
    f32 total;   // 0x40
    u32 row;     // 0x44
    u32 column;  // 0x48
};

struct UnkKraidAI_l5f131c {
    u8 pad_0x0[0x14ce4];
    f32 field_0x14ce4;
    f32 field_0x14ce8;
};

struct UnkAcidBlobMovement_l5f131c {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* charclass;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f dir;  // 0x40
    u8 pad_0x4c[0x30];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x88];
    f32 field_0x108;
    u8 pad_0x10c[0x464];
    u8 field_0x570;
    u8 pad_0x571[0x77];
    s32 field_0x5e8;

    void vf_0x290(UnkActor* owner) {
        reinterpret_cast<void (*)(UnkAcidBlobMovement_l5f131c*, UnkActor*)>(vtable[0x290 / 8])(this, owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkAcidBlobMovement_l5f131c*)>(vtable[0x2c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAcidBlobMovement_l5f131c, field_0x5e8) == 0x5e8, "layout");

struct alignas(8) UnkAcidBlobType_l5f131c {
    u32 id;
};

u64 fn_71005d6c38(UnkAcidBlobsPattern_b45a2* self, u32 row, u32 column);
UnkAcidBlobMovement_l5f131c* fn_71005f190c(UnkActor* actor);  // the blob's "MOVEMENT" component
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);
void fn_7100604cb8(UnkAcidBlobMovement_l5f131c* movement);

extern char g_unk_710164a130[];  // the model point the blobs leave from
extern const UnkVec3f g_unk_710164a138 __attribute__((visibility("hidden")));  // a big blob's turn
extern CStrId g_unk_71726f2788 __attribute__((visibility("hidden")));       // the small blob's actor
extern CStrId g_unk_71726f2790 __attribute__((visibility("hidden")));       // the big blob's actor

// The pattern's entry at (row, column), or null past its end.
inline UnkAcidBlobsEntry_l5f131c* EntryAt(UnkAcidBlobsDef_l5f131c* def, u32 row, u32 column) {
    return reinterpret_cast<UnkAcidBlobsEntry_l5f131c*>(
        fn_71005d6c38(reinterpret_cast<UnkAcidBlobsPattern_b45a2*>(def), row, column));
}

inline UnkActor* SpawnBlob(CStrId& id, UnkAcidBlobType_l5f131c& type) {
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkStringData* name = id.data;
    ++name->reads;
    type.id = g_unk_7172a0ba48;
    return reinterpret_cast<UnkActor*>(fn_710119438c(factory, reinterpret_cast<const char*>(&name->String()), &id,
                                                     reinterpret_cast<const UnkPowerUpLifeType_b52&>(type),
                                                     nullptr));
}

inline void SetBlobAngle(UnkActor* blob, const UnkVec3f* angle, bool flag) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(blob->vtable[0xd8 / 8])(blob, angle, flag);
}

static void LaunchNext(UnkAcidBlobsAttack_l5f131c* self, UnkKraidAI_l5f131c* kraid) {
    UnkAcidBlobsEntry_l5f131c* entry = EntryAt(self->def, self->row, self->column);
    if (!entry) {
        self->column = 0;
        entry = EntryAt(self->def, self->row, 0);
        if (!entry)
            return;
    }
    if (self->timer >= 0.0f && self->timer < entry->time)
        return;

    UnkAcidBlobType_l5f131c small_type, big_type;
    UnkActor* blob;
    f32 speed;
    if (entry->big) {
        blob = SpawnBlob(g_unk_71726f2790, big_type);
        speed = self->def->big_speed;
    } else {
        blob = SpawnBlob(g_unk_71726f2788, small_type);
        speed = self->def->small_speed;
    }
    if (!blob)
        return;
    UnkAcidBlobMovement_l5f131c* movement = fn_71005f190c(blob);
    if (!movement || !movement->charclass)
        return;
    if (!fn_710009d50c(movement->charclass->vf_0x0(), fn_71006023c0()))
        return;

    // Where the blob leaves from: the model's point when it has one, else the owner's position.
    UnkActor* owner = self->owner;
    UnkVec3f from(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkMatrix44 point = {};
    point.m[15] = 1.0f;
    point.m[10] = 1.0f;
    point.m[5] = 1.0f;
    point.m[0] = 1.0f;
    if (fn_71011735e8(static_cast<UnkEntity*>(owner->field_0x228), g_unk_710164a130, &point, 0))
        from = UnkVec3f(point.m[3], point.m[7], point.m[11]);

    // Where it lands: a step of 100 per pattern distance, or at the target.
    f32 base_height = kraid->field_0x14ce4;
    f32 base_x = kraid->field_0x14ce8;
    f32 steps;
    f32 land_x;
    if (entry->distance >= 0) {
        steps = (f32)entry->distance;
        land_x = base_x + steps * -100.0f - 50.0f;
    } else {
        land_x = fn_7100d7c990(self->owner)->field_0x44 - 50.0f;
        steps = (f32)(s32)(fabsf(base_x - land_x) / 100.0f);
    }
    f32 t = steps / 13.0f;
    f32 degrees = self->def->near_angle - t * (self->def->near_angle - self->def->far_angle);
    f32 angle = (f32)(degrees * 3.14159265358979323846 / 180.0);
    if (entry->big)
        speed += speed * (1.0f - t) * 0.7f;

    // The launch speed of the arc through the landing point.
    base::math::CVector2D flat;
    fn_710007b430(&flat, reinterpret_cast<const base::math::CVector3D*>(&from));
    f32 rise = base_height - flat.y;
    f32 run = fabsf(land_x - flat.x);
    f32 num = run * (speed * run);
    rise -= run * tanf(angle);
    f32 c = cosf(angle);
    f32 launch = sqrtf(fabsf(num / (c * (c * (rise * -2.0f)))));

    UnkVec3f dir = fn_710007a390(UnkVec3f(1.0f, 0.0f, 0.0f), angle);
    if (fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2] < 0.0f)
        dir.x = -dir.x;
    f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
    dir.x *= inv;
    dir.y *= inv;
    dir.z *= inv;

    blob->vf_0xd0(&from, true);
    UnkVec3f turn = fn_710007a7f0(dir);
    SetBlobAngle(blob, &turn, true);
    movement->field_0x570 = 0;
    if (entry->big) {
        SetBlobAngle(blob, &g_unk_710164a138, true);
        fn_7100604cb8(movement);
    }
    movement->field_0x5e8 = entry->distance;
    movement->speed = launch;
    movement->field_0x108 = speed;
    movement->dir = dir;
    movement->vf_0x290(self->owner);
    movement->vf_0x2c0();
    self->timer = 0.0f;
    ++self->column;
}

void fn_71005f131c(UnkAcidBlobsAttack_l5f131c* self, f32 dt) {
    fn_71005f201c(reinterpret_cast<UnkKraidAttack*>(self), dt);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai) {
        bool is_kraid = fn_710009d50c(ai->vf_0x0(), fn_71005fac74());
        UnkKraidAI_l5f131c* kraid = is_kraid ? reinterpret_cast<UnkKraidAI_l5f131c*>(ai) : nullptr;
        if (is_kraid && self->active) {
            self->total += dt;
            if (self->timer >= 0.0f)
                self->timer += dt;
            LaunchNext(self, kraid);
        }
    }
    if (self->total >= self->def->duration) {
        base::reflection::refl_EmitU32(self->owner->field_0x220, self->def->end_anim->hash, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769740);
        self->active = false;
        self->total = 0.0f;
    }
}
