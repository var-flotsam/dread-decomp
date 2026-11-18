// CSynchronizedBlockCameraCtrl's unit (tools/tu_map.py unit 1536): yes when the controller is off (+0x10
// clear) or held (+0x2e8); otherwise, with a player, yes when the player faces against the direction at
// +0x31c and stands on the far side of the line at +0x314. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The parameter type CMeleeCameraCtrl's caller declares (src/game/CMeleeCameraCtrl).
struct UnkMeleeCameraCtrl_ovb0088;

struct UnkSyncBlockCameraCtrl_la214fc {
    u8 pad_0x0[0x10];
    bool active;  // 0x10
    u8 pad_0x11[0x2d7];
    bool held;  // 0x2e8
    u8 pad_0x2e9[0x2b];
    f32 line_x;  // 0x314
    u8 pad_0x318[0x4];
    f32 direction;  // 0x31c
};
static_assert(__builtin_offsetof(UnkSyncBlockCameraCtrl_la214fc, held) == 0x2e8, "layout");
static_assert(__builtin_offsetof(UnkSyncBlockCameraCtrl_la214fc, direction) == 0x31c, "layout");

struct UnkSyncBlockPlayer_la214fc {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

// Whether the player faces against the controller's direction from the far side of its line.
static inline bool PlayerBehind(UnkSyncBlockCameraCtrl_la214fc* self) {
    auto* player = reinterpret_cast<UnkSyncBlockPlayer_la214fc*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    if (self->direction > 0.0f) {
        if (0.0f > player->xform.GetAngle().y && self->line_x > player->xform.GetTranslation().x)
            return true;
    }
    if (self->direction < 0.0f) {
        if (player->xform.GetAngle().y > 0.0f && self->line_x < player->xform.GetTranslation().x)
            return true;
    }
    return false;
}

bool fn_7100a214fc(UnkMeleeCameraCtrl_ovb0088* ctrl) {
    auto* self = reinterpret_cast<UnkSyncBlockCameraCtrl_la214fc*>(ctrl);
    return !self->active || self->held || PlayerBehind(self);
}
