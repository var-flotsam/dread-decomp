// CMorphBallMovement's unit (hint only): when the ball is not already falling, starts the fall timer and plays one of the two
// "mball_fall" sounds, picked at random, through the owner's sound part with a set of sound parameters kept in a function-local static.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2772.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

struct UnkBallFallOwner {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;
    u8 pad_0x38[0x7e4 - 0x38];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2620 - 0x7e5];
    f32 field_0x2620;
};

void fn_710094b16c(UnkBallFallOwner* self) {
    static const CFilePathStrId s_fall02("actors/morphball/mball_fall_02.wav", true);
    static const CFilePathStrId s_fall01("actors/morphball/mball_fall_01.wav", true);
    if (self->field_0x7e4 || !(self->field_0x2620 <= 0.0f))
        return;
    self->field_0x2620 = 0.35f;
    static UnkSoundDesc s_params;
    s_params.kind.field_0x0 = 0;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 0.35f;
    s_params.field_0x3c = 500.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.0f;
    if (fn_71012395a0() & 1)
        s_params.path = s_fall02;
    else
        s_params.path = s_fall01;
    fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
}
