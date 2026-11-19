// CVulkranMagmaBallMovementComponent's unit: the hit test. When the ball hits the player and the player
// is in the state at fn_7100623810 (+0x340), it records a bounce (owner position, reversed direction,
// angle) and sends it out through two virtual hooks; anything else goes to the base test. Names and
// layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "game/CProjectileMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkObj;

struct UnkVulkranBallOther_l6dfa88;

struct UnkVulkranBallState_l6dfa88 {
    u8 pad_0x0[0x340];
    bool field_0x340;
};

UnkVulkranBallState_l6dfa88* fn_7100623810(UnkVulkranBallOther_l6dfa88* other);
bool fn_7100ac79d8(void* self, void* actor, void* info);

struct UnkVulkranBallKey_l6dfa88;
__attribute__((visibility("hidden"))) extern UnkVulkranBallKey_l6dfa88 g_unk_710164a808;

struct UnkVulkranBallHeld_l6dfa88 {
    u8 pad_0x0[0x10];
    UnkVulkranBallOther_l6dfa88* other;
};

struct UnkVulkranBallClass_l6dfa88 {
    u8 pad_0x0[0xed];
    bool field_0xed;
};

struct UnkVulkranBallAngles_l6dfa88 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkVulkranBallBounce_l6dfa88 {
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    f32 dir_x;
    f32 dir_y;
    f32 dir_z;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
};

struct UnkVulkranBall_l6dfa88;

struct UnkVulkranBallVtbl_l6dfa88 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkVulkranBall_l6dfa88* self, UnkVulkranBallOther_l6dfa88* other, void* info);
    void* slots_0x2b0[(0x338 - 0x2b0) / 8];
    void (*vf_0x338)(UnkVulkranBall_l6dfa88* self, UnkVulkranBallBounce_l6dfa88* bounce,
                     const UnkVulkranBallAngles_l6dfa88* angles, UnkVulkranBallKey_l6dfa88* key,
                     base::global::CFilePathStrId* path);
};

struct UnkVulkranBall_l6dfa88 {
    UnkVulkranBallVtbl_l6dfa88* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x10c - 0x38];
    f32 velocity_x;  // 0x10c
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x118[0x230 - 0x118];
    UnkVulkranBallHeld_l6dfa88* field_0x230;
    u8 pad_0x238[0x3b0 - 0x238];
    s32 field_0x3b0;
    UnkVulkranBallBounce_l6dfa88 bounce;  // 0x3b4
    u8 pad_0x3e0[0x5d0 - 0x3e0];
    UnkVulkranBallClass_l6dfa88* field_0x5d0;
};
static_assert(__builtin_offsetof(UnkVulkranBall_l6dfa88, field_0x5d0) == 0x5d0, "");

bool fn_71006dfa88(UnkVulkranBall_l6dfa88* self, UnkVulkranBallOther_l6dfa88* other, void* a2, void* a3,
                   void* a4, void* a5) {
    if (!other)
        return false;
    UnkVulkranBallHeld_l6dfa88* held = self->field_0x230;
    if ((held ? held->other : nullptr) == other)
        return false;
    if (reinterpret_cast<UnkVulkranBallOther_l6dfa88*>(fn_7100dec788(g_unk_7101d47310)) != other ||
        fn_7100ac79d8(self, other, a4))
        return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
    UnkVulkranBallState_l6dfa88* state = fn_7100623810(other);
    if (!state || !state->field_0x340)
        return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);

    f32 vx = self->velocity_x;
    UnkActor* owner = self->owner;
    f32 px = owner->field_0x44;
    f32 py = owner->field_0x54;
    f32 vy = self->velocity_y;
    f32 vz = self->velocity_z;
    f32 pz = owner->field_0x64;
    f32 length = sqrtf(vx * vx + vy * vy + vz * vz);
    self->field_0x3b0 = -1;
    UnkVulkranBallBounce_l6dfa88& bounce = self->bounce;
    f32 inv = 1.0f / (length + 0.0001f);
    bounce.pos_x = px;
    bounce.pos_y = py;
    bounce.pos_z = pz;
    bounce.dir_x = -(vx * inv);
    bounce.dir_y = -(vy * inv);
    bounce.dir_z = -(vz * inv);
    bounce.field_0x18 = 0.0f;
    bounce.field_0x1c = 0.0f;
    bounce.field_0x20 = 0.0f;
    bounce.field_0x24 = 0.0f;
    bounce.field_0x28 = 40;
    f32 heading = atan2f(bounce.dir_y, bounce.dir_x);
    UnkVulkranBallAngles_l6dfa88 angles;
    angles.x = 0.0f;
    angles.y = 0.0f;
    angles.z = heading;
    self->vtbl->vf_0x338(self, &bounce, &angles, &g_unk_710164a808, &g_unk_7101d470e8);
    if (!self->field_0x5d0->field_0xed)
        self->vtbl->vf_0x2a8(self, other, a4);
    return false;
}
