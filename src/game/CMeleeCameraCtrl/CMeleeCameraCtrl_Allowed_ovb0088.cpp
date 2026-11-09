// CMeleeCameraCtrl's unit (tools/tu_map.py): whether the melee camera stays out. Yes when fn_7100a214fc
// says so; otherwise it looks at the player (fn_7100dec788): no without one, yes without its object at
// +0x2b0, yes when that object's state (+0x50, its +0x10) is 2, or 3 with the controller's flag (+0x330)
// set and the player's part at +0x2a8 clear at +0x181 and +0x188; otherwise yes unless the player's
// character class (+0x220) plays a CCharClassMeleeCameraAllowedTrack (no without a class). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkMeleeCameraState_ovb0088 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10, read atomically

    s32 Kind() const { return __atomic_load_n(&kind, __ATOMIC_RELAXED); }
};

struct UnkMeleeCameraMode_ovb0088 {
    u8 pad_0x0[0x50];
    UnkMeleeCameraState_ovb0088* state;  // 0x50
};

struct UnkMeleeCameraPart_ovb0088 {
    u8 pad_0x0[0x181];
    bool field_0x181;
    u8 pad_0x182[6];
    bool field_0x188;
};

struct UnkMeleeCameraPlayer_ovb0088 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* cls;  // 0x220
    u8 pad_0x228[0x80];
    UnkMeleeCameraPart_ovb0088* part;  // 0x2a8
    UnkMeleeCameraMode_ovb0088* mode;  // 0x2b0
};

struct UnkMeleeCameraCtrl_ovb0088 {
    u8 pad_0x0[0x330];
    bool field_0x330;
};

bool fn_7100a214fc(UnkMeleeCameraCtrl_ovb0088* self);

bool fn_7100927954(UnkMeleeCameraCtrl_ovb0088* self) {
    if (fn_7100a214fc(self))
        return true;
    auto* player = reinterpret_cast<UnkMeleeCameraPlayer_ovb0088*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkMeleeCameraMode_ovb0088* mode = player->mode;
    if (!mode)
        return true;
    if (UnkMeleeCameraState_ovb0088* state = mode->state) {
        if (state->Kind() == 3 && self->field_0x330) {
            UnkMeleeCameraPart_ovb0088* part = player->part;
            if (part && !part->field_0x181 && !part->field_0x188)
                return true;
        }
        if (state->Kind() == 2)
            return true;
    }
    UnkAnimationComponent* cls = player->cls;
    if (!cls)
        return false;
    return !fn_710117b348(cls, fn_7100d18d48());
}
