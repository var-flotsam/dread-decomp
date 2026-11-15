// nn::atk unit: sets a voice's biquad filter from the coefficients derived for the source's cutoff
// value, or from cleared ones when the value is out of range. Names and layouts are placeholders.
#include "game/UnkAtkSystem.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkBiquadCoeffs {
    s16 value[5];
};

struct UnkAtkBiquadParam {
    u8 enable;
    u8 pad_0x1;
    UnkAtkBiquadCoeffs coeffs;
};

struct UnkAtkFilterSource {
    u8 pad_0x00[0x68];
    u8 field_0x68;
    u8 pad_0x69[0x74 - 0x69];
    u16 field_0x74;
};

struct UnkAtkFilterVoice {
    u8 pad_0x00[0x20];
    u8 voice[0x8c];
    u32 field_0xac;
};

UnkAtkBiquadCoeffs fn_710122afe8(u32 cutoff, u32 value, u32 flag);
void fn_710123a6c0(void* voice, u32 channel, const UnkAtkBiquadParam* param);

static inline void CopyCoeffs(UnkAtkBiquadParam* param, const UnkAtkBiquadCoeffs& c) {
    for (int i = 0; i < 5; i++)
        param->coeffs.value[i] = c.value[i];
}

void fn_710122f9e0(UnkAtkFilterVoice* self, UnkAtkFilterSource* src) {
    UnkAtkBiquadParam param;
    if (static_cast<u16>(src->field_0x74 - 1) <= 0x3e7e) {
        if (static_cast<UnkAtkSystem_b0150*>(fn_71001039f8())->b_956) {
            CopyCoeffs(&param, fn_710122afe8(src->field_0x74, self->field_0xac, 0));
        } else {
            CopyCoeffs(&param, fn_710122afe8(src->field_0x74, static_cast<UnkAtkSystem_b0150*>(fn_71001039f8())->param.sample_rate, 1));
        }
    } else {
        for (int i = 0; i < 5; i++)
            param.coeffs.value[i] = 0;
    }
    param.enable = src->field_0x68;
    fn_710123a6c0(self->voice, 1, &param);
}
