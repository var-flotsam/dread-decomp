// media::CMovieAudioStreamNx (unit 326): sets the stream up for a format and channel count under the mutex at
// +0xcd8: sizes and allocates the source (+0xc50) and its staging buffer, queues the source in chunks, for mono
// allocates a zeroed buffer of +0xd20 bytes, then opens the decoder (fn_7100220d2c) at 48000 Hz.
// Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "game/media/CMovieAudioStreamNx.h"
#include "game/media/MovieBuffer.h"
#include "types.h"

struct UnkAudioStream_l221698 {
    void** vtable;
    u8 pad_0x8[0xc20 - 0x8];
    UnkMovieAudioOut* decoder;  // 0xc20
    u8 pad_0xc28[1];
    bool ready;  // 0xc29
    u8 pad_0xc2a[2];
    s32 sample_rate;     // 0xc2c
    s32 out_channels;    // 0xc30
    s32 format;          // 0xc34
    s32 channels;        // 0xc38
    s32 bytes_per_frame; // 0xc3c
    u8 pad_0xc40[0x10];
    u8* source;          // 0xc50
    s32 source_size;     // 0xc58
    s32 chunks;          // 0xc5c
    s32 chunk_size;      // 0xc60
    s32 field_0xc64;
    bool field_0xc68;
    u8 pad_0xc69[0x7];
    base::global::CRntVector<movie::Buffer> queued;   // 0xc70
    base::global::CRntVector<movie::Buffer> pending;  // 0xc98
    movie::Buffer* staging;  // 0xcc0
    u8 pad_0xcc8[0xcd8 - 0xcc8];
    nn::os::MutexType mutex;  // 0xcd8
    u8 pad_0xcf8[0xd10 - 0xcf8];
    bool resampling;  // 0xd10: set by the decoder's open
    bool mono;        // 0xd11
    u8 pad_0xd12[6];
    void* silence;     // 0xd18
    u64 silence_size;  // 0xd20
};
static_assert(__builtin_offsetof(UnkAudioStream_l221698, mutex) == 0xcd8, "UnkAudioStream_l221698 layout");
static_assert(__builtin_offsetof(UnkAudioStream_l221698, silence_size) == 0xd20, "UnkAudioStream_l221698 layout");

// Holds the mutex for its scope.
struct UnkMutexLock_l221698 {
    nn::os::MutexType* mutex;
    explicit UnkMutexLock_l221698(nn::os::MutexType* m) : mutex(m) { fn_710006691c(mutex); }
    ~UnkMutexLock_l221698() { fn_7100066920(mutex); }
};

static inline s32 Open(UnkAudioStream_l221698* self, s32 format, s32 channels, bool flag) {
    nn::os::MutexType* mutex = &self->mutex;
    UnkMutexLock_l221698 lock(mutex);
    self->format = format;
    self->channels = channels;
    self->out_channels = channels;
    self->field_0xc68 = flag;
    if (channels >= 3) {
        self->chunk_size = 0x8000;
        self->field_0xc64 = 0x3000;
        self->source_size = self->chunks << 15;
    }

    self->staging = new movie::Buffer(self->chunk_size);
    self->staging->SetRange(0, 0);
    self->source = new u8[self->source_size];
    s32 offset = 0;
    for (s32 i = 0; i < self->chunks; ++i) {
        movie::Buffer chunk(self->source + offset, self->chunk_size);
        offset += self->chunk_size;
        self->queued.EmplaceBack(static_cast<movie::Buffer&&>(chunk));
    }

    if (self->channels == 1) {
        self->mono = true;
        self->out_channels = 2;
        self->silence = GetDefaultAllocator()->vf_0x40(self->silence_size, 0x1000, 1);
        if (!self->silence) {
            // The original unlocks here and again on the way out.
            fn_7100066920(mutex);
            return 0x1002;
        }
        memset(self->silence, 0, self->silence_size);
    }

    self->sample_rate = 48000;
    self->bytes_per_frame = self->out_channels * 2;
    s32 result = fn_7100220d2c(self->decoder, self->format, self->out_channels, &self->resampling);
    if (result == 0)
        self->ready = true;
    return result;
}

s32 fn_7100221698(UnkPlayer_7100221938* self, u32 format, u32 channels, bool flag) {
    return Open(reinterpret_cast<UnkAudioStream_l221698*>(self), format, channels, flag);
}
