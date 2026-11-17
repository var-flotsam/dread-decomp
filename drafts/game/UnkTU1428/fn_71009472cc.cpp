// CMorphBallMovement's unit (hint only): plays one of the two spider-ball "oil out" sounds, picked at
// random, through the owner's sound part with a set of sound parameters kept in a function-local static.
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

struct UnkBallSoundOwner {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;
};

void fn_71009472cc(UnkBallSoundOwner* self) {
    static UnkSoundDesc s_params;
    s_params.kind.field_0x0 = 0;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 0.65f;
    s_params.field_0x3c = 500.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.3f;
    static const CFilePathStrId s_out01("actors/morphball/spiderball_oil_out_01.wav", true);
    static const CFilePathStrId s_out02("actors/morphball/spiderball_oil_out_02.wav", true);
    if (fn_71012395a0() & 1)
        s_params.path = s_out02;
    else
        s_params.path = s_out01;
    fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
}
