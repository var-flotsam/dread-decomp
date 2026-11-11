#include "base/global/CFilePathStrId.h"
#include "types.h"

namespace nn::audio {
struct VoiceType;
struct FinalMixType;
struct SubMixType;
void SetVoiceMixVolume(VoiceType* voice, FinalMixType* destination, f32 volume, s32 source, s32 destination_index);
void SetVoiceMixVolume(VoiceType* voice, SubMixType* destination, f32 volume, s32 source, s32 destination_index);
}  // namespace nn::audio

namespace nn::detail {
[[noreturn]] void UnexpectedDefaultImpl(const char* a, const char* b, s32 c);
}  // namespace nn::detail

void* fn_71001039f8();

struct UnkAtkMixSource_b0150 {
    virtual s32 Kind() = 0;          // 0x0
    virtual s32 Channels() = 0;      // 0x8
    virtual s32 Count() = 0;         // 0x10
    u8 pad_0x8[0x38];
    u8 sub_mix[0x10];                // 0x40
    u8 final_mix[0x10];              // 0x50
};

struct UnkAtkSystemFlag_b0150 {
    u8 pad_0x0[0x950];
    bool field_0x950;
};

struct UnkAtkVoice {
    u8 pad_0x0[0x20];
    u8 out[0xa0];                    // 0x20
    UnkAtkMixSource_b0150* source;   // 0xc0
};

void fn_710122faa0(UnkAtkVoice* self, void* params, u32 mode) {
    f32* gains = (f32*)params;
    s32 n1 = 1;
    if (((UnkAtkSystemFlag_b0150*)fn_71001039f8())->field_0x950)
        n1 = self->source->Count();
    s32 n2 = self->source->Channels();
    if (n1 < 1 || n2 < 1)
        return;
    nn::audio::VoiceType* voice = (nn::audio::VoiceType*)self->out;
    for (s32 i = 0; i != n1; i++) {
        for (s32 j = 0; j != n2; j++) {
            s32 idx = i * self->source->Channels() + j;
            f32 volume = 0.0f;
            if (mode == 2 || (u32)j <= 1) {
                f32 g = gains[idx];
                f32 m = __builtin_fminf(g, 128.0f);
                volume = -128.0f;
                if (g >= -128.0f)
                    volume = m;
            }
            s32 kind = self->source->Kind();
            if (kind == 0) {
                nn::audio::SetVoiceMixVolume(voice, (nn::audio::FinalMixType*)self->source->final_mix, volume, 0, idx);
            } else if (kind == 1) {
                nn::audio::SetVoiceMixVolume(voice, (nn::audio::SubMixType*)self->source->sub_mix, volume, 0, idx);
            } else {
                nn::detail::UnexpectedDefaultImpl(g_unk_710162e2ed, g_unk_710162e2ed, 0);
            }
        }
    }
}
