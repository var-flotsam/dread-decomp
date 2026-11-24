// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): when an armed door is idle and
// the player is at least the configured distance away (in x and y), sends the door's animation the id at
// +0xb08 of the game's id table and moves the door's state machine to state 2.
// Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game/CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "base/math/CVector.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkDoorOwner_l8d2d1c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkDoorConfig_l8d2d1c {
    u8 pad_0x0[0x11c];
    bool field_0x11c;
    u8 pad_0x11d[0x124 - 0x11d];
    f32 distance;
    bool field_0x128;
};

// The player as read here: the translation column of its world matrix.
struct UnkDoorPlayer_l8d2d1c {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0x54 - 0x48];
    f32 y;
    u8 pad_0x58[0x64 - 0x58];
    f32 z;
};

struct UnkDoor_l8d2d1c {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d2d1c* owner;
    u8 pad_0x38[0x418 - 0x38];
    bool field_0x418;
    u8 pad_0x419[0x41c - 0x419];
    bool field_0x41c;
    u8 pad_0x41d[0x420 - 0x41d];
    UnkDoorStateMachine_b58 state_machine;
    u8 pad_0x4d0[0x698 - 0x4d0];
    CVector3D position;
    u8 pad_0x6a4[0x9f8 - 0x6a4];
    UnkDoorConfig_l8d2d1c* config;
    u8 pad_0xa00[0xaf8 - 0xa00];
    u8 timer[0x8];
};

void fn_71008d2d1c(UnkDoor_l8d2d1c* self) {
    if (!self->field_0x41c)
        return;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || self->field_0x418)
        return;
    if (fn_710117ae38(animation, GameManagerIdTable().field_0xb10.data->Key(), false))
        return;
    if (fn_710117ae38(animation, GameManagerIdTable().field_0xb08.data->Key(), false))
        return;
    UnkDoorConfig_l8d2d1c* config = self->config;
    if (!config->field_0x11c || config->field_0x128)
        return;
    if (fn_710002fb70(self->timer))
        return;
    UnkDoorPlayer_l8d2d1c* player = reinterpret_cast<UnkDoorPlayer_l8d2d1c*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;

    CVector2D door;
    fn_710007b430(&door, &self->position);
    CVector2D other;
    CVector3D at = {player->x, player->y, player->z};
    fn_710007b430(&other, &at);
    f32 dx = door.x - other.x;
    f32 dy = door.y - other.y;
    if (!(sqrtf(dx * dx + dy * dy) >= self->config->distance))
        return;

    // UnkSpitSend's request (mode 0, all bits), its locals below the distance's.
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(animation, GameManagerIdTable().field_0xb08.data->Key(), 0xf4061ff25a2163d6, &mode,
                  &g_unk_710164b958, &g_unk_7172769740, &mask);
    RequestState_b58(&self->state_machine, 2);
}
