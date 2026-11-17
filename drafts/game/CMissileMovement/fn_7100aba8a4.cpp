// CMissileMovement's unit (tools/tu_map.py unit 1618): the missile's update. Detached, it only moves; otherwise
// it aims at the player's target when locked on, runs the projectile update, ends the missile once it and the
// point behind it leave the bounds, then sets the speed: a launch phase, the burst, and an eased boost. Names
// and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CProjectileMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU341.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;

void* fn_7100ed77a0(void* player);

// The boost tunables.
struct UnkMissileBoost_laba8a4 {
    u8 pad_0x0[0x2c];
    f32 start;  // 0x2c
    f32 end;  // 0x30
};
extern UnkMissileBoost_laba8a4 g_unk_7172727420;

// The speed override (the global tunable g_unk_71726af060 seen with more of its fields).
struct UnkMissileSpeedOverride_laba8a4 {
    u8 pad_0x0[0x34];
    f32 speed;  // 0x34
    bool enabled;  // 0x38
};

struct UnkMissileOwner_laba8a4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x280 - 0x78];
    UnkWorld* world;  // 0x280
};

struct UnkMissileComponents_laba8a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x260 - 0x228];
    void* grab;  // 0x260
};

struct UnkMissileActor_laba8a4 {
    u8 pad_0x0[0x10];
    UnkMissileComponents_laba8a4* components;  // 0x10
};

struct UnkMissileTunables_laba8a4 {
    u8 pad_0x0[0xd4];
    f32 max_speed;  // 0xd4
    u8 pad_0xd8[0x134 - 0xd8];
    f32 speed;  // 0x134
    f32 launch_time;  // 0x138
    f32 boost_time;  // 0x13c
};

struct UnkMissileBounds_laba8a4 {
    void** vtable;
    bool Contains(const CVector2D* p) {
        return reinterpret_cast<bool (*)(UnkMissileBounds_laba8a4*, const CVector2D*)>(vtable[0x50 / 8])(this, p);
    }
};

struct UnkMissileArea_laba8a4 {
    u8 pad_0x0[0x158];
    UnkMissileBounds_laba8a4* bounds;  // 0x158
};

struct UnkMissilePart_laba8a4 {
    void** vtable;
    void SetScale(const CVector3D* scale) {
        reinterpret_cast<void (*)(UnkMissilePart_laba8a4*, const CVector3D*)>(vtable[0x20 / 8])(this, scale);
    }
};

struct UnkMissileMovement_laba8a4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkMissileOwner_laba8a4* owner;  // 0x30
    u8 pad_0x38[0x8];
    CVector3D direction;  // 0x40
    u8 pad_0x4c[0x84 - 0x4c];
    CVector3D position;  // 0x84
    u8 pad_0x90[0x230 - 0x90];
    UnkMissileActor_laba8a4* actor;  // 0x230
    u8 pad_0x238[0x4b8 - 0x238];
    CVector3D velocity;  // 0x4b8
    u8 pad_0x4c4[0x571 - 0x4c4];
    u8 field_0x571;
    u8 pad_0x572[0x5d0 - 0x572];
    bool boosted;  // 0x5d0
    u8 pad_0x5d1[3];
    f32 time;  // 0x5d4
    UnkStringData* ids[3];  // 0x5d8
    UnkMissileTunables_laba8a4* tunables;  // 0x5f0
    bool speed_override;  // 0x5f8
    u8 field_0x5f9;
    u8 pad_0x5fa[0x608 - 0x5fa];
    bool detached;  // 0x608
    u8 pad_0x609[0x618 - 0x609];
    UnkMissileArea_laba8a4* area;  // 0x618

    void vf_0x310(f32 dt, CVector3D* pos) {
        reinterpret_cast<void (*)(UnkMissileMovement_laba8a4*, f32, CVector3D*)>(vtable[0x310 / 8])(this, dt, pos);
    }
    void vf_0x340(CVector3D* pos) {
        reinterpret_cast<void (*)(UnkMissileMovement_laba8a4*, CVector3D*)>(vtable[0x340 / 8])(this, pos);
    }

    UnkMissileComponents_laba8a4* Components() { return actor ? actor->components : nullptr; }
    UnkAnimationComponent* Animation() {
        UnkMissileComponents_laba8a4* components = Components();
        return components ? components->animation : nullptr;
    }

    // Whether a point is inside the area's bounds (anywhere when there are none).
    UnkMissileBounds_laba8a4* Bounds() { return area ? area->bounds : nullptr; }
    bool InBounds(const CVector3D& p) {
        UnkMissileBounds_laba8a4* b = Bounds();
        if (!b)
            return true;
        CVector2D flat;
        fn_710007b430(&flat, &p);
        return b->Contains(&flat);
    }

    void SetVelocity(f32 speed) {
        velocity.x = direction.x * speed;
        velocity.y = direction.y * speed;
        velocity.z = direction.z * speed;
    }
};
static_assert(__builtin_offsetof(UnkMissileMovement_laba8a4, area) == 0x618, "layout");

void fn_7100aba8a4(UnkMissileMovement_laba8a4* self, f32 dt) {
    if (self->detached) {
        CVector3D pos;
        pos.x = self->position.x;
        pos.y = self->position.y;
        pos.z = self->position.z;
        self->vf_0x310(dt, &pos);
        self->vf_0x340(&pos);
        return;
    }

    u8 locked;
    void* target = fn_7100ed77a0(g_unk_71726af0b0);
    UnkAnimationComponent* animation = target ? self->Animation() : nullptr;
    if (animation && fn_710117b348(animation, fn_7100bf4a98())) {
        UnkMatrix44 m = fn_7100d36a2c(static_cast<UnkShape*>(target));
        const UnkMatrix44& own = self->owner->transform;
        f32 dx = m.m[3] - own.m[3];
        f32 dy = m.m[7] - own.m[7];
        f32 dz = m.m[11] - own.m[11];
        f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
        self->field_0x5f9 = self->field_0x571;
        locked = true;
        f32 inv = 1.0f / (length + 0.0001f);
        self->direction.x = dx * inv;
        self->direction.y = dy * inv;
        self->direction.z = dz * inv;
    } else {
        locked = self->field_0x5f9;
    }
    self->field_0x571 = locked;
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);

    // Out of bounds, the missile ends unless the point behind it is still inside.
    if (!self->InBounds(self->position)) {
        UnkMissileBounds_laba8a4* bounds = self->Bounds();
        if (!bounds) {
            fn_7100232c68(self->owner);
            return;
        }
        CVector3D behind;
        behind.x = self->position.x - self->direction.x * 200.0f;
        behind.y = self->position.y - self->direction.y * 200.0f;
        behind.z = self->position.z - self->direction.z * 200.0f;
        CVector2D flat;
        fn_710007b430(&flat, &behind);
        if (!bounds->Contains(&flat)) {
            fn_7100232c68(self->owner);
            return;
        }
    }

    f32 launch_time = self->tunables->launch_time;
    f32 boost_time = self->tunables->boost_time;
    UnkMissileComponents_laba8a4* components = self->Components();
    if (components && components->grab && fn_710113d83c(components->grab)) {
        launch_time = g_unk_7172727420.start;
        boost_time = g_unk_7172727420.end;
    }
    f32 speed = self->tunables->speed;
    f32 max_speed = self->tunables->max_speed;
    if (self->speed_override) {
        f32 ratio = max_speed > 0.0f ? speed / max_speed : 0.0f;
        const UnkMissileSpeedOverride_laba8a4& over =
            reinterpret_cast<const UnkMissileSpeedOverride_laba8a4&>(g_unk_71726af060);
        max_speed = over.speed;
        if (!over.enabled) {
            self->SetVelocity(max_speed);
            return;
        }
        speed = ratio * max_speed;
    }

    f32 time = self->time;
    if (time < launch_time) {
        time += dt;
        self->time = time;
        self->SetVelocity(speed);
        if (time < launch_time)
            return;
        UnkMissileOwner_laba8a4* owner = self->owner;
        auto* part = reinterpret_cast<UnkMissilePart_laba8a4*>(fn_7101152b90(owner->world, self->ids[1]->Key()));
        if (part) {
            CVector3D scale;
            scale.x = 1.1f;
            scale.y = 1.1f;
            scale.z = 1.1f;
            part->SetScale(&scale);
        }
        fn_710114fe8c(self->owner->world, self->ids[2]->Key(), 1, 0);
        return;
    }
    if (time >= launch_time && !self->boosted) {
        time += dt;
        self->time = time;
        if (time <= boost_time) {
            f32 t = sinf(1.0f / (boost_time - launch_time) * (time - launch_time));
            self->SetVelocity(speed + (max_speed - speed) * t);
            return;
        }
        self->boosted = true;
    }
    self->SetVelocity(max_speed);
}
