// Unit 1501 (CSamusMovement): after the base contact handling, walks the contacts for breakable tiles
// that Samus stands or hangs on and, unless she is at rest beside it, starts breaking each one and lands
// her: caps the fall speed, sets the ground timers and the grounded flag. Names and layouts are placeholders.
#include <float.h>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBreakableTileGroup.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkBreakableTile;
struct UnkSamusGunComponent;
void fn_7100c9fe00(UnkBreakableTile* self, s32 kind, s32 mode, bool flag, bool force);
bool fn_710085aab0(UnkSamusGunComponent* self, void* where, char* info);

// The settings word copied into the movement's ground timer.
struct UnkSamusSettings_l9e6fdc {
    u8 pad_0x0[0x7c];
    u32 ground_time;  // 0x7c
};

struct UnkContactLink_l9e6fdc {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
};

struct UnkContactSource_l9e6fdc {
    u8 pad_0x0[0x10];
    UnkContactLink_l9e6fdc* link;
};

// One contact of the movement (0x48 bytes): where it came from, the tile key and its normal.
struct UnkContact_l9e6fdc {
    UnkContactSource_l9e6fdc* source;
    u8 pad_0x8[0xc];
    u8 at[4];  // 0x14
    f32 height;  // 0x18
    u8 pad_0x1c[8];
    f32 normal_y;  // 0x24
    u8 pad_0x28[8];
    f32 normal_x;  // 0x30
    u8 pad_0x34[0x14];
};
static_assert(sizeof(UnkContact_l9e6fdc) == 0x48, "layout");

struct UnkContacts_l9e6fdc {
    UnkContact_l9e6fdc* data;
    u64 count;
};

// A breakable tile as the query returns it.
struct UnkTile_l9e6fdc {
    u8 pad_0x0[0x28];
    f32 top;  // 0x28
    u8 pad_0x2c[0xc];
    s32 kind;  // 0x38
};

struct UnkMoveInput_l9e6fdc {
    u8 pad_0x0[4];
    f32 height;  // 0x4
    u8 pad_0x8[0x20];
    f32 velocity_x;  // 0x28
    f32 velocity_y;  // 0x2c
    f32 velocity_z;  // 0x30
};

struct UnkMoveOutput_l9e6fdc {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    u8 pad_0x28[8];
    f32 fall_speed;  // 0x30
    u8 pad_0x34[4];
    f32 height;  // 0x38
};

struct UnkTileBreaker_l9e6fdc;
void fn_7100301f50(UnkTileBreaker_l9e6fdc* breaker, UnkQueryObj* tile, UnkReflObject* group, bool flag);

struct UnkOwner_l9e6fdc {
    u8 pad_0x0[0x54];
    f32 step_height;  // 0x54
    u8 pad_0x58[0x220 - 0x58];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkReflObject* gun;  // 0x2a8
    u8 pad_0x2b0[0x2e0 - 0x2b0];
    UnkTileBreaker_l9e6fdc* breaker;  // 0x2e0
};

struct UnkTunables_l9e6fdc {
    u8 pad_0x0[0xc0];
    f32 ground_delay;  // 0xc0
};

struct UnkSamusMovement_l9e6fdc;
struct UnkSamusMovementVtable_l9e6fdc {
    void* slots_0x0[0x260 / 8];
    void (*OnGrounded)(UnkSamusMovement_l9e6fdc* self);  // 0x260
    void* slots_0x268[(0x2b8 - 0x268) / 8];
    void (*OnLeaveWall)(UnkSamusMovement_l9e6fdc* self);  // 0x2b8
};

struct UnkSamusMovement_l9e6fdc {
    UnkSamusMovementVtable_l9e6fdc* vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l9e6fdc* owner;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    bool grounded;  // 0xf9
    u8 pad_0xfa[0x1dc - 0xfa];
    f32 max_fall_speed;  // 0x1dc
    u8 pad_0x1e0[0x200 - 0x1e0];
    f32 ground_delay;  // 0x200
    u8 pad_0x204[0x3a8 - 0x204];
    bool on_wall;  // 0x3a8
    u8 pad_0x3a9[3];
    f32 land_time;  // 0x3ac
    u8 pad_0x3b0[0x7c8 - 0x3b0];
    f32 air_land_time;  // 0x7c8
    u8 pad_0x7cc[0x51ac - 0x7cc];
    u32 ground_time;  // 0x51ac
    u8 pad_0x51b0[0x5458 - 0x51b0];
    UnkTunables_l9e6fdc* tunables;  // 0x5458

    void SetOnWall(bool value) {
        if (on_wall != value) {
            on_wall = value;
            vtable->OnLeaveWall(this);
        }
    }
    void SetGrounded(bool value) {
        if (grounded != value) {
            grounded = value;
            vtable->OnGrounded(this);
        }
    }
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9e6fdc, air_land_time) == 0x7c8, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e6fdc, ground_time) == 0x51ac, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e6fdc, tunables) == 0x5458, "layout");

void fn_71010dbd54(UnkSamusMovement_l9e6fdc* self, UnkContacts_l9e6fdc* contacts, UnkMoveInput_l9e6fdc* in,
                   UnkMoveOutput_l9e6fdc* out);

void fn_71009e6fdc(UnkSamusMovement_l9e6fdc* self, UnkContacts_l9e6fdc* contacts, UnkMoveInput_l9e6fdc* in,
                   UnkMoveOutput_l9e6fdc* out) {
    fn_71010dbd54(self, contacts, in, out);
    for (UnkContact_l9e6fdc *c = contacts->data, *end = c + contacts->count; c != end; ++c) {
        UnkReflObject* group = c->source->link->object;
        if (!group || !fn_710009d50c(group->vf_0x0(), fn_7100ca0484()))
            continue;
        UnkQueryObj* query = fn_7100ca2048(group, c->at, 1);
        if (!query)
            continue;
        UnkTile_l9e6fdc* tile = reinterpret_cast<UnkTile_l9e6fdc*>(query);

        // The gun gets the first try at a tile not yet broken.
        UnkReflObject* gun = self->owner->gun;
        bool is_gun = gun && fn_710009d50c(gun->vf_0x0(), fn_7100857564());
        if (is_gun && tile->kind != 2)
            fn_710085aab0(reinterpret_cast<UnkSamusGunComponent*>(gun), group, reinterpret_cast<char*>(c));
        if (tile->kind == 2 || fn_7100c9e960(query) != 7)
            continue;

        // Only a tile under her feet: a floor, or a flat enough contact no higher than a step.
        if (!((c->normal_x >= FLT_EPSILON && c->normal_y <= -FLT_EPSILON) ||
              (c->normal_x >= -FLT_EPSILON && c->normal_y <= FLT_EPSILON &&
               c->height <= self->owner->step_height)))
            continue;

        // At rest at the same height, a tile not just below her is left alone.
        bool moving = UNK_CHANGED(in->velocity_x, 0.0f) || UNK_CHANGED(in->velocity_y, 0.0f) ||
                      UNK_CHANGED(in->velocity_z, 0.0f);
        if (!moving && in->height + 0.1f >= out->height && in->height - 0.1f <= out->height) {
            f32 depth = out->height - tile->top;
            if (!(depth > 0.0f && depth < 100.0f))
                continue;
        }

        bool alt = fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        fn_7100c9fe00(reinterpret_cast<UnkBreakableTile*>(query), alt, 0, false, false);
        fn_7100301f50(self->owner->breaker, query, group, alt);
        UnkAnimationComponent* animation = self->owner->animation;
        bool hanging = animation && fn_710117b348(animation, fn_7100d029c8());
        if (alt || hanging)
            continue;

        if (!(self->land_time > 0.0f)) {
            if (!self->grounded)
                self->air_land_time = 0.6f;
            self->land_time = 0.6f;
        }
        f32 fall = out->field_0x24 < -300.0f ? out->field_0x24 : -300.0f;
        fall = out->fall_speed < fall ? out->fall_speed : fall;
        fall = self->max_fall_speed < fall ? self->max_fall_speed : fall;
        out->field_0x24 = 0.0f;
        out->fall_speed = fall;
        self->ground_time = reinterpret_cast<UnkSamusSettings_l9e6fdc*>(&g_unk_7172740de0)->ground_time;
        self->SetOnWall(false);
        self->SetGrounded(true);
        self->ground_delay = self->tunables->ground_delay + FLT_EPSILON;
    }
}
