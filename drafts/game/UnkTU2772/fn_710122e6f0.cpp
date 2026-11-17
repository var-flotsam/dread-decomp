// Unit 2772 (nn::atk): computes a voice's output gains. When the voice's outputs enable the given bus, each
// channel's level (the first four from the parameters, the first one plus one; the rest from the
// controller) is limited to [0, 1] or to at least 0, and every output gain is the volume times that
// level times the given gain, scaled by the controller's factor where it applies. Names and layouts are
// placeholders.
#include <algorithm>

#include "game/UnkTU99.h"
#include "types.h"

// The renderer: how many outputs a channel has (vf_0x8), how many channels (vf_0x10), and whether a
// channel's level is limited to 1 (vf_0x20).
class UnkMixRenderer_l22e6f0 {
public:
    virtual void vf_0x0();
    virtual s32 vf_0x8();
    virtual s32 vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20(s32 channel);
};

struct UnkMixVoice_l22e6f0 {
    u8 pad_0x0[0x208];
    u32 outputs;  // 0x208: one bit per output
    u8 pad_0x20c[0x280 - 0x20c];
    UnkMixRenderer_l22e6f0* renderer;  // 0x280
};

struct UnkMixParams_l22e6f0 {
    f32 volume;
    u8 pad_0x4[0x3c];
    f32 levels[4];  // 0x40
};

// The output table fn_71001039f8 gives: the buses each output feeds.
struct UnkMixOutputs_l22e6f0 {
    u8 pad_0x0[0x510];
    u8 buses[32];  // 0x510
};

struct UnkMixController_l22e6f0;
bool fn_710120f368(UnkMixController_l22e6f0* ctrl);
f32 fn_710120f340(UnkMixController_l22e6f0* ctrl, s32 channel);
bool fn_710120f488(UnkMixController_l22e6f0* ctrl);
bool fn_710120f450(UnkMixController_l22e6f0* ctrl, s32 channel);
f32 fn_710120f434(UnkMixController_l22e6f0* ctrl);

static inline f32 Clamp01(f32 v) {
    return v > 1.0f ? 1.0f : std::max(v, 0.0f);
}

void fn_710122e6f0(UnkMixVoice_l22e6f0* self, f32* gains, s32 bus, const UnkMixParams_l22e6f0* params,
                   UnkMixController_l22e6f0* ctrl, const f32* sends) {
    s32 channels = self->renderer->vf_0x10();
    f32 v;
    const f32 floor = 0.0f;
    f32 levels[24];
    for (s32 i = 0; i < channels; i++)
        levels[i] = 0.0f;

    u32 buses = 0;
    for (u64 i = 0; i < 32; i++) {
        if (self->outputs & (1 << i))
            buses |= static_cast<UnkMixOutputs_l22e6f0*>(fn_71001039f8())->buses[i];
    }

    if (buses & (1 << bus)) {
        for (s32 i = 0; i < channels; i++) {
            if (i < 4) {
                v = params->levels[i];
                if (i == 0)
                    v += 1.0f;
                levels[i] = self->renderer->vf_0x20(i) ? Clamp01(v) : std::max(v, floor);
            } else if (ctrl != nullptr && fn_710120f368(ctrl)) {
                if (self->renderer->vf_0x20(i)) {
                    levels[i] = Clamp01(fn_710120f340(ctrl, i));
                } else {
                    v = fn_710120f340(ctrl, i);
                    levels[i] = std::max(v, floor);
                }
            } else {
                break;
            }
        }
    }

    for (s32 ch = 0; ch < channels; ch++) {
        for (s32 j = 0; j < std::min(self->renderer->vf_0x8(), 6); j++) {
            s32 k = ch * self->renderer->vf_0x8() + j;
            f32 gain = params->volume * levels[ch] * sends[k];
            if (ctrl && fn_710120f488(ctrl) && !fn_710120f450(ctrl, ch))
                gain *= fn_710120f434(ctrl);
            gains[k] = gain;
        }
    }
}
