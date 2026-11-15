// CSamusMovement's unit (unit 1501): eases the run speed (+0x1d8) towards the track's speed limit while the
// movement is grounded and its timer (+0x7c) is at zero. Names and layouts are placeholders.
#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;
struct UnkAbility;
struct UnkMove_b152;
struct UnkVec2_b152;

bool fn_71009e73cc(UnkMove_b152* self, UnkVec2_b152* out);

struct UnkSpeedLimit_l9e307c {
    f32 speed;
    f32 scale;
};

struct UnkMovement_l9e307c {
    u8 pad_0x0[0x7c];
    f32 field_0x7c;  // 0x7c
    u8 pad_0x80[0x79];
    bool field_0xf9;  // 0xf9
    u8 pad_0xfa[0xde];
    f32 field_0x1d8;  // 0x1d8
    u8 pad_0x1dc[0x2148];
    u8 field_0x2324;  // 0x2324
};
static_assert(__builtin_offsetof(UnkMovement_l9e307c, field_0x2324) == 0x2324, "layout");

static inline bool IsNearZero(f32 value) {
    const f32 epsilon = 1e-5f;
    return value - epsilon <= 0.0f && value + epsilon >= 0.0f;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71009e307c(UnkMovement_l9e307c* self, const f32* amount) {
    UnkSpeedLimit_l9e307c limit = {0.0f, 10.0f};
    if (!self->field_0xf9 || !IsNearZero(self->field_0x7c) || self->field_0x2324)
        return;
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    if (ability && fn_710033cf20(ability))
        return;
    fn_71009e73cc(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkVec2_b152*>(&limit));
    f32 speed = self->field_0x1d8;
    f32 over = __builtin_fabsf(speed) - limit.speed;
    if (!(over > 1.1920929e-7f))
        return;
    f32 t = *amount * limit.scale;
    t = t < 1.0f ? t : 1.0f;
    f32 keep = 1.0f - t;
    s32 sign = (speed > 0.0f) - (speed < 0.0f);
    self->field_0x1d8 = (limit.speed + over * keep) * sign;
}
