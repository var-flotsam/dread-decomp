// CChozoCommanderEnergyShardsSphereMovementComponent's unit (tools/tu_map.py): the update. Held (state 0),
// the sphere sits on its holder's node g_unk_7101649718 (else the holder's position and facing), on the
// z = 0 plane. Launched (state 1), it turns towards its goal by at most `dt` per axis and slows as it
// nears it; once there it bursts on a timer (fn_710045a2fc) a limited number of times, then ends.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CProjectileMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU341.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern char g_unk_7101649718[];

struct UnkShardsSphereHolder_l459fb4 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkShardsSphereDef_l459fb4 {
    u8 pad_0x0[0x13c];
    s32 bursts;          // 0x13c: negative for no limit
    f32 burst_interval;  // 0x140
    u8 pad_0x144[4];
    f32 life_limit;  // 0x148
};

// What the owner's +0x1b8 points to.
struct UnkShardsSphereLife_l459fb4 {
    u8 pad_0x0[0x74];
    f32 value;  // 0x74
};

struct UnkShardsSphereMovement_l459fb4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 dir_x;  // 0x40
    f32 dir_y;  // 0x44
    f32 dir_z;  // 0x48
    u8 pad_0x4c[0x30];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x1b0];
    UnkShardsSphereHolder_l459fb4* holder;  // 0x230
    u8 pad_0x238[0x398];
    UnkShardsSphereDef_l459fb4* def;  // 0x5d0
    f32 goal_x;  // 0x5d8
    f32 goal_y;  // 0x5dc
    f32 goal_z;  // 0x5e0
    u8 pad_0x5e4[0x10];
    f32 burst_timer;  // 0x5f4
    s32 burst_count;  // 0x5f8
    s32 field_0x5fc;
    s32 state;  // 0x600
};
static_assert(__builtin_offsetof(UnkShardsSphereMovement_l459fb4, def) == 0x5d0, "layout");
static_assert(__builtin_offsetof(UnkShardsSphereMovement_l459fb4, state) == 0x600, "layout");

void fn_710045a2fc(UnkShardsSphereMovement_l459fb4* self);

// Moves `cur` towards `target` by at most `step`.
inline f32 StepToward_l459fb4(f32 cur, f32 target, f32 step) {
    f32 lo = cur - step;
    f32 hi = cur + step;
    f32 down = target > lo ? target : lo;
    f32 up = target < hi ? target : hi;
    return !(target > cur) ? down : up;
}

inline f32 Clamp01_l459fb4(f32 v) {
    return v > 0.0f ? (!(v >= 1.0f) ? v : 1.0f) : 0.0f;
}

// The angles of the direction a transform matrix faces (its third column).
inline UnkVec3f FacingAngles_l459fb4(const UnkMatrix44& m) {
    return fn_710007a7f0(UnkVec3f(m.m[2], m.m[6], m.m[10]));
}

inline void SetAngles_l459fb4(UnkActor* actor, const UnkVec3f* angles, bool flag) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(actor->vtable[0xd8 / 8])(actor, angles, flag);
}

void fn_7100459fb4(UnkShardsSphereMovement_l459fb4* self, f32 dt) {
    switch (self->state) {
    case 0: {
        UnkShardsSphereHolder_l459fb4* holder = self->holder;
        UnkActor* actor = holder->actor;
        UnkVec3f pos(actor->field_0x44, actor->field_0x54, actor->field_0x64);
        const UnkMatrix44& xf =
            fn_710002614c(reinterpret_cast<const UnkMatrix44*>((holder ? holder->actor : nullptr)->field_0x38));
        UnkVec3f angles = FacingAngles_l459fb4(xf);
        UnkMatrix44 m = {};
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        if (fn_71011735e8(static_cast<UnkEntity*>(self->holder->actor->field_0x228), g_unk_7101649718, &m, 0)) {
            pos = UnkVec3f(m.m[3], m.m[7], m.m[11]);
            angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
        }
        pos.z = 0.0f;
        self->owner->vf_0xd0(&pos, true);
        SetAngles_l459fb4(self->owner, &angles, true);
        break;
    }
    case 1: {
        UnkActor* owner = self->owner;
        f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
        f32 dx = self->goal_x - ox;
        f32 dy = self->goal_y - oy;
        f32 dz = self->goal_z - oz;
        f32 dist2 = dx * dx + dy * dy + dz * dz;
        if (!(dist2 <= 400.0f)) {
            f32 inv = 1.0f / (__builtin_sqrtf(dist2) + 0.0001f);
            f32 x = StepToward_l459fb4(self->dir_x, dx * inv, dt);
            f32 y = StepToward_l459fb4(self->dir_y, dy * inv, dt);
            self->dir_x = x;
            self->dir_y = y;
            self->dir_z = StepToward_l459fb4(self->dir_z, dz * inv, dt);
            f32 near = Clamp01_l459fb4(dist2 / 160000.0f);
            self->speed = near >= 1.0f ? 1.0f : near + (1.0f - near) * 0.15f;
            fn_7100ac38fc(self);
            fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
            return;
        }
        f32 timer = self->burst_timer - dt;
        self->burst_timer = timer;
        if (timer > 0.0f)
            return;
        fn_710045a2fc(self);
        UnkShardsSphereLife_l459fb4* life = static_cast<UnkShardsSphereLife_l459fb4*>(self->owner->field_0x1b8);
        UnkShardsSphereDef_l459fb4* def = self->def;
        if (life && life->value >= def->life_limit) {
            fn_7100232c68(self->owner);
            return;
        }
        if (def->bursts < 0) {
            self->field_0x5fc = 0;
        } else if (self->burst_count++ >= def->bursts) {
            fn_7100232c68(self->owner);
            return;
        }
        self->burst_timer = def->burst_interval;
        break;
    }
    }
}
