// CMorphBallMovement's unit (hint only): plays one of the two spider-ball "oil in" sounds, picked at
// random, through the owner's sound part with a set of sound parameters kept in a function-local static.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2772.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters a sound is played with (only the ids are released at exit).
struct UnkBallSoundParams {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    CStrId id = g_unk_7101d470e0;
    CFilePathStrId path = g_unk_7101d470e8;
    f32 field_0x28 = 0.0f;
    f32 field_0x2c = 0.0f;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = -1.0f;
    f32 field_0x38 = 1.0f;
    f32 field_0x3c = 500.0f;
    f32 field_0x40 = 2000.0f;
    f32 field_0x44 = 1.0f;
    f32 field_0x48 = 0.0f;
    f32 field_0x4c = 0.0f;
    f32 field_0x50 = 0.0f;
    f32 field_0x54 = -1.0f;
    f32 field_0x58 = -1.0f;
    f32 field_0x5c = -1.0f;
    f32 field_0x60 = 0.5f;
    f32 field_0x64 = 5.0f;
    f32 field_0x68 = 10.0f;
    bool field_0x6c = false;
    bool field_0x6d = false;
    bool field_0x6e = false;

    ~UnkBallSoundParams();  // fn_710034e0b8
};
static_assert(sizeof(UnkBallSoundParams) == 0x70);

struct UnkBallSoundOwner {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;
};

void fn_7100947780(UnkBallSoundOwner* self) {
    static const CFilePathStrId s_in01("actors/morphball/spiderball_oil_in_01.wav", true);
    static const CFilePathStrId s_in02("actors/morphball/spiderball_oil_in_02.wav", true);
    static UnkBallSoundParams s_params;
    s_params.field_0x0 = 0;
    s_params.field_0x6d = true;
    s_params.field_0x38 = 0.55f;
    s_params.field_0x3c = 500.0f;
    s_params.field_0x40 = 50000.0f;
    s_params.field_0x44 = 1.0f;
    if (fn_71012395a0() & 1)
        s_params.path = s_in02;
    else
        s_params.path = s_in01;
    fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
}
