// Unit 1125: the step after the base's (fn_71007038c0). With a movement at the owner's +0x218, the
// velocity (+0x100) restarts from the owner's (fn_7100d7be08), takes this step's acceleration
// (fn_7100703fec), is limited at the owner's position (fn_710070419c), and, when the movement is in
// mode 3, is sent to it as a kind-3 block: the direction and the speed as a share of its maximum.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" f32 sqrtf(f32);

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkSteerVec_l703dd4 {
    f32 x;
    f32 y;
    f32 z;
};

// The owner's movement (+0x218).
struct UnkSteerMovement_l703dd4 {
    u8 pad_0x0[0x3cc];
    u32 limit_mode;  // 0x3cc
};

struct UnkSteer_l703dd4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xc8];
    UnkSteerVec_l703dd4 velocity;  // 0x100
};

void fn_71007038c0(UnkSteer_l703dd4* self, f32 dt);
UnkKraidCreatureVec fn_7100703fec(UnkSteer_l703dd4* self);
void fn_710070419c(UnkSteer_l703dd4* self, const CVector2D* at, u32 mode, UnkSteerVec_l703dd4* velocity, f32 dt,
                   f32 limit);
f32 fn_71010d0c64(void* movement);

struct UnkOptF32_l703dd4 {
    f32 value;
    bool has;
};

struct UnkOptVec_l703dd4 {
    UnkSteerVec_l703dd4 value;
    bool has;

    void Set(const UnkSteerVec_l703dd4& v) {
        value = v;
        has = true;
    }
};

// The kind-3 block handed to the movement (fn_71010c7e6c).
struct UnkSteerParams_l703dd4 {
    void** vtable;
    UnkOptF32_l703dd4 speed;      // 0x8
    UnkOptF32_l703dd4 field_0x10;
    UnkOptVec_l703dd4 direction;  // 0x18
    UnkOptVec_l703dd4 facing;     // 0x28
    u32 kind;                     // 0x38
    u8 pad_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkSteerParams_l703dd4, kind) == 0x38, "layout");

static inline CVector3D Position(const UnkActor* actor) {
    CVector3D pos = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    return pos;
}

static inline f32 Length(const UnkSteerVec_l703dd4& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

static inline UnkSteerVec_l703dd4 Normalized(const UnkSteerVec_l703dd4& v) {
    f32 inv = 1.0f / (Length(v) + 0.0001f);
    UnkSteerVec_l703dd4 n;
    n.x = v.x * inv;
    n.y = v.y * inv;
    n.z = inv * v.z;
    return n;
}

void fn_7100703dd4(UnkSteer_l703dd4* self, f32 dt) {
    fn_71007038c0(self, dt);
    auto* movement = static_cast<UnkSteerMovement_l703dd4*>(self->owner->field_0x218);
    if (!movement)
        return;
    CVector2D at;
    {
        UnkKraidCreatureVec start = fn_7100d7be08(self->owner);
        self->velocity.x = start.x;
        self->velocity.y = start.y;
        self->velocity.z = start.z;
    }
    UnkKraidCreatureVec accel = fn_7100703fec(self);
    {
        CVector3D pos = Position(self->owner);
        fn_710007b430(&at, &pos);
    }
    f32 dx = accel.x * dt;
    f32 dy = accel.y * dt;
    f32 dz = accel.z * dt;
    f32 x = dx + self->velocity.x;
    f32 y = dy + self->velocity.y;
    f32 z = dz + self->velocity.z;
    self->velocity.x = x;
    self->velocity.y = y;
    self->velocity.z = z;
    fn_710070419c(self, &at, movement->limit_mode, &self->velocity, dt,
                  2000.0f);
    UnkSteerVec_l703dd4 direction = Normalized(self->velocity);
    f32 speed = Length(self->velocity);
    f32 max = fn_71010d0c64(movement);
    if (fn_71010cc3a4(movement) != 3)
        return;
    f32 share = speed / max;
    UnkSteerParams_l703dd4 params;
    params.field_0x10.has = false;
    params.field_0x3d = false;
    params.speed.value = !(share <= 0.0f) ? (!(share >= 1.0f) ? share : 1.0f) : 0.0f;
    params.vtable = g_unk_7101a2e208 + 2;
    params.kind = 3;
    params.facing.Set(direction);
    params.direction.Set(direction);
    params.speed.has = true;
    fn_71010c7e6c(static_cast<UnkEffectSink*>(self->owner->field_0x218), &params, 1);
}
