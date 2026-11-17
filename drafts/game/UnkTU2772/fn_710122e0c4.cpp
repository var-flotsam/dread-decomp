// Unit 2772 (nn::atk): computes a voice channel's six output volumes (front left and right, rear left
// and right, centre, LFE), either as given or from its pan and surround pan under the voice's pan
// curve, downmixes them for the output's mode, and writes them per output channel (the controller's own
// value where it overrides one). Names and layouts are placeholders.
#include "game/UnkTU99.h"
#include "types.h"

// How a pan is turned into gains: the curve (square root, sine-cosine or linear), 0 dB at the centre,
// clamped at 0 dB, and the front bypass.
struct UnkPanInfo_l22e0c4 {
    u32 curve;
    bool center_zero_db;  // 0x4
    bool zero_db_clamp;   // 0x5
    bool pad_0x6;
    bool front_bypass;  // 0x7

    UnkPanInfo_l22e0c4() : curve(0), center_zero_db(false), zero_db_clamp(false), pad_0x6(false), front_bypass(false) {}
};

f32 fn_710122b164(const UnkPanInfo_l22e0c4* info, u32 mode, f32 pan);
f32 fn_710122b204(const UnkPanInfo_l22e0c4* info, f32 span);

struct UnkMixVolume_l22e0c4 {
    f32 front_left;
    f32 front_right;
    f32 rear_left;
    f32 rear_right;
    f32 center;
    f32 lfe;
};

struct UnkMixParams_l22e0c4 {
    u32 pad_0x0;
    u32 use_mix_volume;               // 0x4
    UnkMixVolume_l22e0c4 volumes[2];  // 0x8
    f32 pan;                          // 0x38
    f32 span;                         // 0x3c
};

class UnkMixRenderer_l22e0c4 {
public:
    virtual void vf_0x0();
    virtual s32 vf_0x8();
    virtual s32 vf_0x10();
};

struct UnkMixVoice_l22e0c4 {
    u8 pad_0x0[0x1c0];
    s32 field_0x1c0;    // 0x1c0: which side a lone channel plays on (1 left, 2 right)
    s32 channel_count;  // 0x1c4
    u8 pad_0x1c8[0x1f4 - 0x1c8];
    s32 field_0x1f4;  // 0x1f4
    u32 pan_curve;    // 0x1f8
    u8 pad_0x1fc[0x280 - 0x1fc];
    UnkMixRenderer_l22e0c4* renderer;  // 0x280
};

// The sound system fn_71001039f8 gives: each output's mix mode (0 mono, 1 stereo, 2 surround) and
// downmix kind.
struct UnkMixSystem_l22e0c4 {
    u8 pad_0x0[0xe0];
    u32 mix_mode[1];  // 0xe0
    u32 downmix[1];   // 0xe4
    u8 pad_0xe8[0xa65 - 0xe8];
    bool front_bypass;  // 0xa65
};

struct UnkMixController_l22e0c4;
bool fn_710120f414(UnkMixController_l22e0c4* ctrl);
bool fn_710120f3d4(UnkMixController_l22e0c4* ctrl);
bool fn_710120f3f0(UnkMixController_l22e0c4* ctrl, s32 channel);
f32 fn_710120f3ac(UnkMixController_l22e0c4* ctrl, s32 source_channel, s32 output);

static inline UnkMixSystem_l22e0c4* MixSystem() {
    return static_cast<UnkMixSystem_l22e0c4*>(fn_71001039f8());
}

static inline f32 GetVolume(const UnkMixVolume_l22e0c4& mix, s32 index) {
    switch (index) {
    case 0:
        return mix.front_left;
    case 1:
        return mix.front_right;
    case 2:
        return mix.rear_left;
    case 3:
        return mix.rear_right;
    case 4:
        return mix.center;
    case 5:
        return mix.lfe;
    default:
        return 0.0f;
    }
}

void fn_710122e0c4(UnkMixVoice_l22e0c4* self, f32* out, const UnkMixParams_l22e0c4* params,
                   UnkMixController_l22e0c4* ctrl, s32 channel, u32 output) {
    u32 mode = MixSystem()->mix_mode[output];
    UnkMixSystem_l22e0c4* system = MixSystem();
    UnkMixVolume_l22e0c4 mix;
    if (!params->use_mix_volume) {
        UnkPanInfo_l22e0c4 info;
        if (system->front_bypass)
            info.front_bypass = true;
        switch (self->pan_curve) {
        case 0:
            info.curve = 0;
            break;
        case 1:
            info.curve = 0;
            info.center_zero_db = true;
            break;
        case 2:
            info.curve = 0;
            info.center_zero_db = true;
            info.zero_db_clamp = true;
            break;
        case 3:
            info.curve = 1;
            break;
        case 4:
            info.curve = 1;
            info.center_zero_db = true;
            break;
        case 5:
            info.curve = 1;
            info.center_zero_db = true;
            info.zero_db_clamp = true;
            break;
        case 6:
            info.curve = 2;
            break;
        case 7:
            info.curve = 2;
            info.center_zero_db = true;
            break;
        case 8:
            info.curve = 2;
            info.center_zero_db = true;
            info.zero_db_clamp = true;
            break;
        default:
            info.curve = 0;
            break;
        }

        f32 left = 0.0f;
        f32 right = 0.0f;
        f32 front = 0.0f;
        f32 rear = 0.0f;
        if (mode == 1 || mode == 2) {
            if (self->channel_count >= 2 && self->field_0x1f4 == 1) {
                if (channel == 0)
                    left = fn_710122b164(&info, mode, params->pan);
                else if (channel == 1)
                    right = fn_710122b164(&info, mode, 0.0f - params->pan);
            } else if (self->field_0x1c0 != 0 && self->field_0x1f4 != 0 && !MixSystem()->front_bypass) {
                if (self->field_0x1c0 == 1)
                    left = fn_710122b164(&info, mode, params->pan);
                else
                    right = fn_710122b164(&info, mode, 0.0f - params->pan);
            } else {
                f32 pan = params->pan;
                if (channel == 0 && self->channel_count == 2) {
                    pan += -1.0f;
                } else if (channel == 1 && self->channel_count == 2) {
                    if (self->field_0x1c0 == 1)
                        pan += -1.0f;
                    else
                        pan += 1.0f;
                } else if (self->field_0x1c0 == 1) {
                    pan += -1.0f;
                } else if (self->field_0x1c0 == 2) {
                    pan += 1.0f;
                }
                left = fn_710122b164(&info, mode, pan);
                right = fn_710122b164(&info, mode, 0.0f - pan);
            }
        } else if (mode == 0) {
            left = right = fn_710122b164(&info, mode, 0.0f);
        }
        if (mode == 2) {
            front = fn_710122b204(&info, params->span);
            rear = fn_710122b204(&info, 2.0f - params->span);
        } else if (mode == 0 || mode == 1) {
            front = fn_710122b204(&info, 0.0f);
            rear = fn_710122b204(&info, 2.0f);
        }
        mix.front_left = left * front;
        mix.front_right = right * front;
        mix.rear_left = left * rear;
        mix.rear_right = right * rear;
        mix.center = 0.0f;
        mix.lfe = 0.0f;
    } else {
        mix = params->volumes[channel];
    }

    u32 downmix = MixSystem()->downmix[output];
    if (downmix == 1) {
        if (mode == 2) {
            mix.front_right = mix.rear_right + mix.front_right;
            mix.rear_right = 0.0f;
            mix.front_left = mix.rear_left + mix.front_left;
            mix.rear_left = 0.0f;
        }
    } else if (downmix == 0) {
        if (mode == 2) {
            f32 sum = (mix.rear_right + mix.front_right) + (mix.rear_left + mix.front_left);
            mix.rear_right = 0.0f;
            mix.rear_left = 0.0f;
            mix.front_left = mix.front_right = sum * 0.5f;
        } else if (mode == 1) {
            mix.front_left = mix.front_right = (mix.front_right + mix.front_left) * 0.5f;
        }
    }

    for (s32 i = 0; i < 24; i++) {
        s32 ch = i / self->renderer->vf_0x8();
        f32 value = 0.0f;
        if (ch < self->renderer->vf_0x10()) {
            if (ctrl && fn_710120f414(ctrl) && fn_710120f3d4(ctrl) && fn_710120f3f0(ctrl, ch))
                value = fn_710120f3ac(ctrl, channel, i);
            else
                value = GetVolume(mix, i % self->renderer->vf_0x8());
        }
        out[i] = value;
    }
}
