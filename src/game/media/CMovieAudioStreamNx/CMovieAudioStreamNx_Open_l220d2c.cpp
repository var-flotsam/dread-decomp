// media::CMovieAudioStreamNx (unit 326): opens the movie's audio output, sets up a resampler when the
// output's rate differs from the movie's, and queues four zeroed buffers. Names and layouts are placeholders.
#include <stddef.h>

#ifdef NNSDK
#include <nn/audio/audio_SampleFormat.h>
#else
// The port's runtime has no audio_SampleFormat.h; only the type is used here.
namespace nn::audio {
enum SampleFormat : int;
}  // namespace nn::audio
#endif

#include "base/AudioOutParameter.h"
#include "types.h"

void* fn_71001324b0(u64 align, u64 size);

// The audio output types; the matching build's SDK headers have none (as in src/base/snd/CSoundSystemATK.cpp).
namespace nn::audio {
struct AudioOut {
    u8 storage[0x168];
};
struct AudioOutBuffer {
    u8 storage[0x28];
};
struct ResamplerType {
    u8 storage[0x20];
};
void InitializeAudioOutParameter(AudioOutParameter* parameter);
s32 OpenDefaultAudioOut(AudioOut* out, const AudioOutParameter& parameter);
s32 GetAudioOutChannelCount(const AudioOut* out);
SampleFormat GetAudioOutSampleFormat(const AudioOut* out);
size_t GetSampleByteSize(SampleFormat format);
s32 GetAudioOutSampleRate(const AudioOut* out);
size_t GetRequiredBufferSizeForResampler(s32 channelCount);
s32 InitializeResampler(ResamplerType* resampler, void* buffer, size_t size, s32 inputRate, s32 outputRate,
                        s32 channelCount);
s32 ProcessResamplerBuffer(ResamplerType* resampler, s32* outSampleCount, s16* out, size_t outSize, const s16* in,
                           s32 inSampleCount);
void SetAudioOutBufferInfo(AudioOutBuffer* buffer, void* data, size_t bufferSize, size_t dataSize);
bool AppendAudioOutBuffer(AudioOut* out, AudioOutBuffer* buffer);
}  // namespace nn::audio

struct UnkMovieAudioOut {
    s32 channel_count;  // 0x0
    s32 sample_rate;    // 0x4
    s32 field_0x8;
    s32 field_0xc;
    u8 pad_0x10[8];
    size_t sample_size;  // 0x18
    size_t buffer_size;  // 0x20
    nn::audio::AudioOutBuffer buffers[4];  // 0x28
    nn::audio::AudioOut out;               // 0xc8
    nn::audio::SampleFormat format;        // 0x230
    u8 pad_0x234[4];
    nn::audio::ResamplerType resampler;  // 0x238
    void* in_buffers[4];                 // 0x258
    void* resampler_work;                // 0x278
    void* out_buffers[4];                // 0x280
    s32 input_rate;                      // 0x2a0
    s32 input_channels;                  // 0x2a4
    u8 pad_0x2a8[9];
    bool opened;      // 0x2b1
    bool resampling;  // 0x2b2
};
static_assert(__builtin_offsetof(UnkMovieAudioOut, format) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkMovieAudioOut, opened) == 0x2b1, "layout");

s32 fn_7100220d2c(UnkMovieAudioOut* self, s32 sampleRate, s32 channels, bool* outResampling) {
    if (outResampling == nullptr)
        return 0x100b;

    s32 outChannels = self->channel_count;
    self->sample_rate = sampleRate;
    self->field_0x8 = channels;
    self->input_rate = sampleRate;
    self->input_channels = channels;
    self->field_0xc = outChannels * 2;
    if (outChannels < channels)
        self->channel_count = channels;

    nn::audio::AudioOutParameter parameter = {};
    nn::audio::InitializeAudioOutParameter(&parameter);
    parameter.sample_rate = 48000;
    parameter.channel_count = self->channel_count;
    if (nn::audio::OpenDefaultAudioOut(&self->out, parameter) == 0)
        self->channel_count = nn::audio::GetAudioOutChannelCount(&self->out);
    self->format = nn::audio::GetAudioOutSampleFormat(&self->out);
    self->sample_size = nn::audio::GetSampleByteSize(self->format);

    s32 outputRate = nn::audio::GetAudioOutSampleRate(&self->out);
    if (self->sample_rate != outputRate) {
        size_t workSize = nn::audio::GetRequiredBufferSizeForResampler(self->input_channels);
        self->resampler_work = fn_71001324b0(0x1000, workSize);
        if (self->resampler_work == nullptr)
            return 0x1002;
        __builtin_memset(self->resampler_work, 0, workSize);
        nn::audio::InitializeResampler(&self->resampler, self->resampler_work, workSize, self->sample_rate, 48000,
                                       self->input_channels);
        self->resampling = true;
    }

    s32 outSamples = 0;
    for (s32 i = 0; i < 4; ++i) {
        self->in_buffers[i] = fn_71001324b0(0x1000, self->buffer_size);
        if (self->in_buffers[i] == nullptr)
            return 0x1002;
        __builtin_memset(self->in_buffers[i], 0, self->buffer_size);
        if (self->resampling) {
            self->out_buffers[i] = fn_71001324b0(0x1000, 0);
            if (self->out_buffers[i] == nullptr)
                return 0x1002;
            nn::audio::ProcessResamplerBuffer(&self->resampler, &outSamples, static_cast<s16*>(self->out_buffers[i]),
                                              0, static_cast<const s16*>(self->in_buffers[i]), 0x204);
            s32 dataSize = outSamples * channels * nn::audio::GetSampleByteSize(self->format);
            nn::audio::SetAudioOutBufferInfo(&self->buffers[i], self->out_buffers[i], 0, dataSize);
        } else {
            nn::audio::SetAudioOutBufferInfo(&self->buffers[i], self->in_buffers[i], self->buffer_size, 0x204);
        }
        nn::audio::AppendAudioOutBuffer(&self->out, &self->buffers[i]);
    }

    *outResampling = self->resampling;
    self->opened = true;
    return 0;
}
