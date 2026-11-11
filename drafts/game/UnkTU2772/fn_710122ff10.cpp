// nn::atk-ish unit 2772: voice pool update pass. Names and layouts are placeholders.
#include <nn/audio.h>

#include "types.h"

struct UnkAtkWaveVoice_b0149;
void fn_710122fce4(UnkAtkWaveVoice_b0149* self);

struct UnkAtkWaveNode_b0268 {
    u8 pad_0x0[0x10];
    s64 end;  // 0x10
    s64 start;  // 0x18
    u8 pad_0x20[0x10];
    bool loop;  // 0x30
};

struct UnkAtkUpdateItem_b0268 {
    u8 pad_0x0[0x20];
    u8 voice[0x8];  // 0x20, nn::audio::VoiceType
    u8 field_0x28;
    u8 field_0x29;
    u8 pad_0x2a[0x7e];
    u32 field_0xa8;
    u8 pad_0xac[0xc];
    s64 field_0xb8;
    u8 pad_0xc0[8];
    UnkAtkWaveNode_b0268* waves;  // 0xc8
};

struct UnkAtkUpdatePool_b0268 {
    u8 pad_0x0[8];
    UnkAtkUpdateItem_b0268** slots;  // 0x8
    s32 count;  // 0x10
    u8 pad_0x14[0x10];
    u32 field_0x24;
};

static inline nn::audio::VoiceType* Voice(UnkAtkUpdateItem_b0268* item) {
    return reinterpret_cast<nn::audio::VoiceType*>(item->voice);
}

void fn_710122ff10(UnkAtkUpdatePool_b0268* self) {
    u32 released = 0;
    for (s64 i = 0; i < self->count; i++) {
        UnkAtkUpdateItem_b0268* first = self->slots[i];
        if (nn::audio::IsVoiceValid(Voice(first)) && nn::audio::IsVoiceDroppedFlagOn(Voice(first))) {
            UnkAtkUpdateItem_b0268* item = self->slots[i];
            if (item->field_0x28) {
                item->field_0x28 = 0;
                released++;
                continue;
            }
        }
        UnkAtkUpdateItem_b0268* item = self->slots[i];
        if (!nn::audio::IsVoiceValid(Voice(item)))
            continue;
        u32 state = nn::audio::GetVoicePlayState(Voice(item));
        if (item->field_0xa8 | state)
            continue;
        fn_710122fce4((UnkAtkWaveVoice_b0149*)item);
        if (!item->field_0x29)
            continue;
        s64 played = nn::audio::GetVoicePlayedSampleCount(Voice(item));
        UnkAtkWaveNode_b0268* wave = item->waves;
        s64 base = wave ? wave->start : 0;
        if (wave && wave->loop) {
            s64 length = wave->end - base;
            if (played > length)
                played %= length;
        }
        item->field_0xb8 = played + base;
    }
    __atomic_store_n(&self->field_0x24, released, __ATOMIC_RELEASE);
}
