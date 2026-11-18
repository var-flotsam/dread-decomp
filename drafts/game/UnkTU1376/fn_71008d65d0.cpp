// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): fn_71008d65d0 decides whether a hit
// opens the door from one side: the hit must come from the left or the right (its direction, or where its
// source stands), its damage must be one the side accepts (charge beam damage only with the charge beam), and
// the player must stand on that side. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CInventoryComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkInventory;

struct UnkHitCharClass_l8d65d0 : UnkReflObject {
    u8 pad_0x8[0x98 - 0x8];
    CStrId field_0x98;
};

struct UnkHitEntity_l8d65d0 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
    u8 pad_0x30[0x44 - 0x30];
    f32 x;  // 0x44
    u8 pad_0x48[0x54 - 0x48];
    f32 y;  // 0x54
    u8 pad_0x58[0x64 - 0x58];
    f32 z;  // 0x64
};

struct UnkHitSource_l8d65d0 {
    u8 pad_0x0[0x10];
    UnkHitEntity_l8d65d0* entity;  // 0x10
};

struct UnkDoorHit_l8d65d0 {
    u8 pad_0x0[0x8];
    UnkHitSource_l8d65d0* source;  // 0x8
    u8 pad_0x10[0x84 - 0x10];
    f32 dir_x;  // 0x84
    f32 dir_y;  // 0x88
    f32 dir_z;  // 0x8c
    u8 pad_0x90[0x94 - 0x90];
    u8 flags;  // 0x94: 0x10 the direction is set
    u8 pad_0x95[0xb4 - 0x95];
    u32 damage;  // 0xb4: damage kind bits
};

struct UnkDoorLifeState_l8d65d0 {
    u8 pad_0x0[0x10];
    u32 id;
};

struct UnkDoorLife_l8d65d0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x41c - 0x38];
    bool field_0x41c;
    u8 pad_0x41d[0x430 - 0x41d];
    UnkDoorLifeState_l8d65d0* current;  // 0x430: the state machine's current state
    u8 pad_0x438[0x644 - 0x438];
    u32 left_damage;   // 0x644: the damage kinds that open it from the left
    u32 right_damage;  // 0x648: and from the right
    u8 pad_0x64c[0x698 - 0x64c];
    f32 x;      // 0x698
    f32 y;      // 0x69c
    f32 z;      // 0x6a0
    f32 width;  // 0x6a4
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d65d0, width) == 0x6a4, "layout");

bool fn_71008e22e4(UnkDoorLife_l8d65d0* self, UnkDoorHit_l8d65d0* hit);
bool fn_71008d6a34(UnkDoorLife_l8d65d0* self);

static inline bool IsNear(f32 value, f32 target) {
    f32 low = value - 0.001f;
    f32 high = value + 0.001f;
    return low <= target && target <= high;
}

static inline f32 Dot(f32 x, f32 y, f32 z, f32 ox, f32 oy, f32 oz) {
    return x * ox + y * oy + z * oz;
}

static inline bool IsCharClass(UnkReflObject* object) {
    return object && fn_710009d50c(object->vf_0x0(), fn_71011abbd4());
}

// Whether the hit comes from the side: anything but straight up or down.
static inline bool FromSide(const UnkDoorHit_l8d65d0* hit) {
    if (!(hit->flags & 0x10))
        return true;
    f32 up = Dot(hit->dir_x, hit->dir_y, hit->dir_z, 0.0f, 1.0f, 0.0f);
    return !IsNear(up, 1.0f) && !IsNear(up, -1.0f);
}

// Whether the player has the charge beam.
static inline bool HasChargeBeam() {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_717275a518);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_717275a518), base::global::CStrId::Empty());
    return fn_71011428bc(inventory, qid);
}

bool fn_71008d65d0(UnkDoorLife_l8d65d0* self, UnkDoorHit_l8d65d0* hit) {
    bool hurt = fn_71008e22e4(self, hit);
    if (self->current && self->current->id == 3)
        return false;
    if (fn_71008d6a34(self) || !self->field_0x41c)
        return false;

    // A hit straight up or down only counts with damage of kind 0x20.
    const u32& kinds = hit->damage;
    if (!(hurt && FromSide(hit)) && !(kinds & 0x20))
        return false;

    f32 dx, dy, dz;
    if (hit->flags & 0x10) {
        dx = hit->dir_x;
        dy = hit->dir_y;
        dz = hit->dir_z;
    } else {
        UnkHitEntity_l8d65d0* entity = hit->source->entity;
        dx = self->x - entity->x;
        dy = self->y - entity->y;
        dz = self->z - entity->z;
    }
    if (hit->source && hit->source->entity && IsCharClass(hit->source->entity->char_class)) {
        UnkReflObject* object = hit->source->entity->char_class;
        auto* char_class = static_cast<UnkHitCharClass_l8d65d0*>(IsCharClass(object) ? object : nullptr);
        if (char_class->field_0x98 == GameManagerIdTable().field_0x1c68) {
            // Of this class, only where the source stands across the door counts.
            f32 half = self->width * 0.5f;
            f32 source_x = hit->source->entity->x;
            dy = 0.0f;
            if (source_x > half + self->x)
                dx = 1.0f;
            else if (source_x < self->x - half)
                dx = -1.0f;
            else
                return false;
            dz = dy;
        }
    }

    f32 side = Dot(dx, dy, dz, 1.0f, 0.0f, 0.0f);
    bool left;
    if (side < -0.2f)
        left = true;
    else if (side > 0.2f)
        left = false;
    else
        return false;

    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    UnkActor* owner = self->owner;
    f32 px = player->field_0x44 - owner->field_0x44;
    f32 py = player->field_0x54 - owner->field_0x54;
    f32 pz = player->field_0x64 - owner->field_0x64;
    f32 length = sqrtf(px * px + py * py + pz * pz);
    u32 damage = kinds;
    px *= 1.0f / (length + 0.0001f);
    if (left) {
        if (!(self->left_damage & damage))
            return false;
        bool usable = !(damage & 0x100) || HasChargeBeam();
        return usable & signbit(px);
    }
    if (!(self->right_damage & damage))
        return false;
    bool usable = !(damage & 0x100) || HasChargeBeam();
    return usable & !signbit(px);
}
