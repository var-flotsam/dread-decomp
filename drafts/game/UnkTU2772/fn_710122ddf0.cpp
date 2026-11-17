// nn::atk unit: copies a voice's mix parameters (an enable flag, a per-bus mask and two gain tables sized by
// the output's channel layout) into its current set, marking the voice dirty (bit 3) for each value that
// changes. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkMix_l22ddf0 {
    f32 mainGains[24];
    f32 auxGains[24];
    u8 enabled;
    u8 pad_0xc1[7];
    u8* busMask;
    s32 busCount;
};

struct UnkAtkOutput_l22ddf0 {
    virtual void vf_0x0();
    virtual s32 vf_0x8();
    virtual s32 vf_0x10();
};

struct UnkAtkVoice_l22ddf0 {
    u8 pad_0x000[0x1e8];
    u16 dirtyFlags;
    u8 pad_0x1ea[0x280 - 0x1ea];
    UnkAtkOutput_l22ddf0* output;
};

static inline void UpdateFlag(u16& dirtyFlags, u8& current, u8 value) {
    if ((value != 0) != (current != 0)) {
        current = value;
        dirtyFlags |= 8;
    }
}

static inline void UpdateGain(u16& dirtyFlags, f32& current, f32 value) {
    if (current != value) {
        current = value;
        dirtyFlags |= 8;
    }
}

void fn_710122ddf0(UnkAtkVoice_l22ddf0* self, const UnkAtkMix_l22ddf0* flags, const UnkAtkMix_l22ddf0* gains,
                   UnkAtkMix_l22ddf0* current) {
    u8& enabled = current->enabled;
    UpdateFlag(self->dirtyFlags, enabled, flags->enabled);
    if (!enabled)
        return;

    s32 flagsCount = flags->busCount;
    s32 busCount = current->busCount < flagsCount ? current->busCount : flagsCount;
    for (s32 i = 0; i < busCount; ++i)
        UpdateFlag(self->dirtyFlags, current->busMask[i], flags->busMask[i]);

    s32 channels = self->output->vf_0x10();
    s32 count = self->output->vf_0x8() * channels;
    for (s32 i = 0; i < count; ++i)
        UpdateGain(self->dirtyFlags, current->mainGains[i], gains->mainGains[i]);
    for (s32 i = 0; i < count; ++i)
        UpdateGain(self->dirtyFlags, current->auxGains[i], gains->auxGains[i]);
}
