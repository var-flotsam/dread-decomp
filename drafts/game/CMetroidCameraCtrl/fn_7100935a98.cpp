// CMetroidCameraCtrl's unit (unit 1424): resets the camera's tuning to its defaults and puts its eye and
// target on the player: at the morph ball's position when the player is in morph ball, else at the
// player's camera point with the eye 28 units lower. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;

struct UnkCameraVec3_l935a98 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkCameraVec2_l935a98 {
    f32 x;
    f32 y;
};

struct UnkPlayerMovement_l935a98 {
    void** vtable;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkPlayerMovement_l935a98*)>(vtable[0])(this);
    }
};

struct UnkCameraPlayer_l935a98 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x188];
    UnkPlayerMovement_l935a98* movement;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkCameraPlayer_l935a98, movement) == 0x1f0, "layout");

struct UnkMetroidCameraCtrl_l935a98 {
    u8 pad_0x0[0x238];
    UnkCameraVec3_l935a98 eye;     // 0x238
    UnkCameraVec3_l935a98 target;  // 0x244
    u8 pad_0x250[0x90];
    UnkCameraVec2_l935a98 field_0x2e0;
    f32 field_0x2e8;
    UnkCameraVec2_l935a98 field_0x2ec;
    UnkCameraVec2_l935a98 field_0x2f4;
    bool field_0x2fc;
    bool field_0x2fd;
    u8 pad_0x2fe[0x2];
    u32 field_0x300;
};
static_assert(__builtin_offsetof(UnkMetroidCameraCtrl_l935a98, field_0x300) == 0x300, "layout");

// The defaults of the pairs at +0x2e0, +0x2ec and +0x2f4.
static const UnkCameraVec2_l935a98 s_default_0x2e0 = {280.0f, 3.0f};
static const UnkCameraVec2_l935a98 s_default_0x2ec = {20.0f, 20.0f};
static const UnkCameraVec2_l935a98 s_default_0x2f4 = {1.0f, 1.0f};

void fn_7100935a98(UnkMetroidCameraCtrl_l935a98* self) {
    self->field_0x2e0 = s_default_0x2e0;
    self->field_0x2e8 = 3.0f;
    self->field_0x2ec = s_default_0x2ec;
    self->field_0x2f4 = s_default_0x2f4;
    self->field_0x2fc = false;
    self->field_0x2fd = false;
    self->field_0x300 = 0;
    auto* player = reinterpret_cast<UnkCameraPlayer_l935a98*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkPlayerMovement_l935a98* movement = player->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_710093b204())) {
        self->target.x = player->transform[0][3];
        self->target.y = player->transform[1][3];
        self->target.z = player->transform[2][3];
        self->eye = self->target;
    } else {
        const UnkCameraVec3_l935a98* pos = reinterpret_cast<const UnkCameraVec3_l935a98*>(
            fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(player)));
        self->target = *pos;
        self->eye = self->target;
        self->eye.y -= 28.0f;
        self->target.y = self->eye.y;
    }
    self->eye.z = 0.0f;
    self->target.z = 0.0f;
}
