// nn::atk unit: copies a voice's output parameters into its current set (a non-negative level, a mode, the
// values only that mode uses and four common values), marking the voice dirty (bit 3) for each value that
// changes. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkOutputParams_l22d8d8 {
    f32 level;
    u32 mode;
    f32 modeOneValues[12];
    f32 modeZeroValues[2];
    f32 commonValues[4];
};

struct UnkAtkDirtyFlags_l22d8d8 {
    u16 flag0 : 1;
    u16 flag1 : 1;
    u16 flag2 : 1;
    u16 outputParams : 1;
    u16 unk_bits : 12;
};

struct UnkAtkVoice_l22d8d8 {
    u8 pad_0x000[0x1e8];
    UnkAtkDirtyFlags_l22d8d8 dirty;
};

template <typename T>
static inline void UpdateValue(UnkAtkVoice_l22d8d8* self, T& current, const T& value) {
    if (current != value) {
        current = value;
        self->dirty.outputParams = true;
    }
}

void fn_710122d8d8(UnkAtkVoice_l22d8d8* self, const UnkAtkOutputParams_l22d8d8* params,
                   UnkAtkOutputParams_l22d8d8* current) {
    f32 level = params->level;
    f32 clamped = !(level < 0.0f) ? level : 0.0f;
    UpdateValue(self, current->level, clamped);
    UpdateValue(self, current->mode, params->mode);
    if (current->mode == 1) {
        for (s32 i = 0; i < 12; ++i)
            UpdateValue(self, current->modeOneValues[i], params->modeOneValues[i]);
    }
    if (current->mode == 0) {
        for (s32 i = 0; i < 2; ++i)
            UpdateValue(self, current->modeZeroValues[i], params->modeZeroValues[i]);
    }
    for (s32 i = 0; i < 4; ++i)
        UpdateValue(self, current->commonValues[i], params->commonValues[i]);
}
