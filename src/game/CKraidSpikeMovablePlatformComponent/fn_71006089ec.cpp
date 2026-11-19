// CKraidSpikeMovablePlatformComponent unit. fn_71006089ec: the platform's movement step. After the base
// step (fn_7100952688) it clamps the step along x: while rising (state 0) it stops at the top (+0x270)
// and starts the owner's wobble; while falling it stops on the floor (+0x25c) and starts the platform
// (fn_7100608bb4); otherwise it does not move. Then it sets flag 8 of the collider unless the player's
// life component reports fn_7101161088. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkWobbleOwner_ovb0032;
struct UnkVec3fCopy_ovb0032;
void fn_7101172cec(UnkWobbleOwner_ovb0032* self, f32 speed, f32 base_x, f32 spread_x, f32 base_y, f32 spread_y,
                   f32 base_z, f32 spread_z, const CStrId& name, const UnkVec3fCopy_ovb0032& a,
                   const UnkVec3fCopy_ovb0032& b);
extern const UnkVec3fCopy_ovb0032 g_unk_710164a2b0 __attribute__((visibility("hidden")));

struct UnkSpikePlatformStart_b59a4;
void fn_7100608bb4(UnkSpikePlatformStart_b59a4* self);

struct UnkKraidSpikeOwner_l6089ec {
    u8 pad_0x0[0x44];
    f32 field_0x44;  // the transform's x translation
    u8 pad_0x48[0x228 - 0x48];
    UnkWobbleOwner_ovb0032* wobble;  // 0x228
};

struct UnkKraidSpikeConfig_l6089ec {
    u8 pad_0x0[0x7c];
    f32 field_0x7c;
    f32 wobble_speed;  // 0x80
    f32 wobble_base_x;
    f32 wobble_spread_x;
    f32 wobble_base_y;
    f32 wobble_spread_y;  // 0x90
};

struct UnkKraidSpikeCollider_l6089ec;
struct UnkKraidSpikeColliderVtable_l6089ec {
    u8 pad_0x0[0x58];
    u32 (*GetFlags)(UnkKraidSpikeCollider_l6089ec* self);  // 0x58
    void (*SetFlags)(UnkKraidSpikeCollider_l6089ec* self, u32 flags);  // 0x60
    u8 pad_0x68[0x98 - 0x68];
    void (*Update)(UnkKraidSpikeCollider_l6089ec* self);  // 0x98
};
struct UnkKraidSpikeCollider_l6089ec {
    const UnkKraidSpikeColliderVtable_l6089ec* vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[0x4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    bool dirty;  // 0x60
};

struct UnkKraidSpikePlayer_l6089ec {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
};

// The platform's state and the time spent in it.
struct UnkKraidSpikeState_l6089ec {
    f32 time;
    u32 id;  // 0 rising, 1 at rest, otherwise falling
};

struct UnkKraidSpikePlatform_l6089ec {
    u8 pad_0x0[0x30];
    UnkKraidSpikeOwner_l6089ec* owner;  // 0x30
    u8 pad_0x38[0x250 - 0x38];
    UnkKraidSpikeConfig_l6089ec* config;  // 0x250
    u32 field_0x258;
    f32 floor;  // 0x25c
    bool started;  // 0x260
    u8 pad_0x261[0x7];
    bool field_0x268;
    u8 pad_0x269[0x7];
    f32 top;  // 0x270
    u8 pad_0x274[0x8];
    UnkKraidSpikeState_l6089ec state;  // 0x27c
};

void fn_7100952688(UnkKraidSpikePlatform_l6089ec* self, UnkVec3f* delta);
UnkKraidSpikeCollider_l6089ec* fn_7100951f20(UnkKraidSpikePlatform_l6089ec* self);

// NON_MATCHING(functional): Conditions differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71006089ec(UnkKraidSpikePlatform_l6089ec* self, UnkVec3f* delta) {
    fn_7100952688(self, delta);
    if (self->state.id != 1) {
    f32 x = self->owner->field_0x44 + delta->x;
    switch (self->state.id) {
    case 0: {
        f32 gap = self->top - self->config->field_0x7c - x;
        if (gap > 0.0f) {
            delta->x += gap;
            self->field_0x258 = 0;
            self->state = UnkKraidSpikeState_l6089ec{0.0f, 1};
            if (UnkWobbleOwner_ovb0032* wobble = self->owner->wobble) {
                UnkKraidSpikeConfig_l6089ec* c = self->config;
                UnkVec3f axis(1.0f, 0.0f, 0.0f);
                fn_7101172cec(wobble, c->wobble_speed, c->wobble_base_x, c->wobble_spread_x, c->wobble_base_y,
                              c->wobble_spread_y, 0.0f, 0.0f, g_unk_7101d470e0,
                              reinterpret_cast<const UnkVec3fCopy_ovb0032&>(axis), g_unk_710164a2b0);
            }
        }
        break;
    }
    default: {
        if (self->field_0x268 || self->started) {
            *delta = UnkVec3f(0.0f, 0.0f, 0.0f);
        } else {
            f32 half_height = 0.0f;
            if (UnkKraidSpikeCollider_l6089ec* collider = fn_7100951f20(self)) {
                if (collider->dirty)
                    collider->vtable->Update(collider);
                half_height = (collider->field_0x48 - collider->field_0x40) * 0.5f;
            }
            f32 gap = self->floor - (x - half_height);
            if (gap > 0.0f) {
                delta->x = gap + delta->x;
                fn_7100608bb4(reinterpret_cast<UnkSpikePlatformStart_b59a4*>(self));
            }
        }
        break;
    }
    }
    }
    auto* player = reinterpret_cast<UnkKraidSpikePlayer_l6089ec*>(fn_7100dec788(g_unk_7101d47310));
    if (UnkKraidSpikeCollider_l6089ec* collider = fn_7100951f20(self)) {
        u32 flags = collider->vtable->GetFlags(collider);
        if (player && player->life && fn_7101161088(player->life))
            flags &= ~8u;
        else
            flags |= 8;
        collider->vtable->SetFlags(collider, flags);
    }
}
