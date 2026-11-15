// nn::atk unit 2772: takes an audio voice slot for a sound voice by its sample mode, routes it to the owner's
// mix and keeps the voice's node id. Answers whether a slot was available. Names and layouts are placeholders.
#include <nn/audio.h>
#include <nn/util.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkTU99.h"
#include "types.h"

namespace nn::audio {
void SetVoiceSrcQuality(VoiceType* voice, s32 quality);
}  // namespace nn::audio

struct UnkAtkMixOwner_l22f5f8 {
    virtual s32 Kind();  // 0x0
    u8 pad_0x8[0x38];
    u8 final_mix[0x10];  // 0x40
    u8 sub_mix[0x10];    // 0x50
};

struct UnkAtkSoundVoice_l22f5f8 {
    u8 pad_0x0[0x20];
    u8 voice[0x8];  // 0x20, nn::audio::VoiceType
    u8 pad_0x28[0x7c];
    s32 priority;  // 0xa4
    u8 pad_0xa8[0x4];
    s32 sample_rate;  // 0xac
    s32 mode;         // 0xb0
    u8 pad_0xb4[0xc];
    UnkAtkMixOwner_l22f5f8* owner;  // 0xc0
    u8 pad_0xc8[0x20];
    u32 node_id;  // 0xe8
};

struct UnkAtkSystemFlags_l22f5f8 {
    u8 pad_0x0[0x958];
    u8 quality_flag;  // 0x958
};

static inline nn::audio::AudioRendererConfig* Config() {
    return reinterpret_cast<nn::audio::AudioRendererConfig*>(static_cast<u8*>(fn_71001039f8()) + 0x18);
}

bool fn_710122f5f8(UnkAtkSoundVoice_l22f5f8* self) {
    nn::audio::SampleFormat format;
    const void* buffer = nullptr;
    size_t buffer_size = 0;
    switch (self->mode) {
    case 0:
        format = nn::audio::SampleFormat_PcmInt8;
        break;
    case 2:
        format = nn::audio::SampleFormat_Adpcm;
        buffer_size = 0x20;
        buffer = self;
        break;
    default:
        format = nn::audio::SampleFormat_PcmInt16;
        break;
    }
    s32 priority = self->priority == 0xff ? 0 : 0xff - self->priority;
    nn::audio::VoiceType* voice = reinterpret_cast<nn::audio::VoiceType*>(self->voice);
    if (!nn::audio::AcquireVoiceSlot(Config(), voice, self->sample_rate, 1, format, priority, buffer, buffer_size).IsSuccess())
        return false;
    if (static_cast<UnkAtkSystemFlags_l22f5f8*>(fn_71001039f8())->quality_flag)
        nn::audio::SetVoiceSrcQuality(voice, 1);
    switch (self->owner->Kind()) {
    case 0:
        nn::audio::SetVoiceDestination(Config(), voice,
                                       reinterpret_cast<nn::audio::SubMixType*>(self->owner->sub_mix));
        break;
    case 1:
        nn::audio::SetVoiceDestination(Config(), voice,
                                       reinterpret_cast<nn::audio::FinalMixType*>(self->owner->final_mix));
        break;
    default:
        nn::detail::UnexpectedDefaultImpl(g_unk_710162e2ed, g_unk_710162e2ed, 0);
    }
    self->node_id = nn::audio::GetVoiceNodeId(voice);
    return true;
}
