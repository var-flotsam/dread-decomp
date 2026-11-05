// media::CMovieAudioStreamNx (unit 326): fn_710022162c, a worker loop: until the owner's state at
// +0x1e4 reads 3 (acquire load), feeds the stream (fn_7100221d48) while it is stopping (+0xc28) and
// bit 2 of its flags at +0x40 is clear, then calls fn_71000810b0 with the stream's word at +0xcc8
// (a wait). Names and layouts are placeholders.
#include "types.h"

struct UnkMovieAudioStream_b68a1 {
    u8 pad_0x0[0x40];
    u8 flags;  // 0x40
    u8 pad_0x41[0xbe7];
    bool stopping;  // 0xc28
    u8 pad_0xc29[0x9f];
    u64 interval;  // 0xcc8
};
static_assert(__builtin_offsetof(UnkMovieAudioStream_b68a1, interval) == 0xcc8, "layout");

struct UnkMovieAudioWorker_b68a1 {
    u8 pad_0x0[0x1e4];
    s32 state;  // 0x1e4
    u8 pad_0x1e8[8];
    UnkMovieAudioStream_b68a1* stream;  // 0x1f0
};

// fn_71000810b0 (declared `void* fn_71000810b0(u32 value)` in src/engine/HostArguments.cpp) under a
// local name taking the whole word: the original loads the 64-bit interval as it is.
void* SleepFor_b68a1(u64 value) asm("_Z13fn_71000810b0j");
void fn_7100221d48(UnkMovieAudioStream_b68a1* stream);

void fn_710022162c(UnkMovieAudioWorker_b68a1* self) {
    UnkMovieAudioStream_b68a1* stream = self->stream;
    while (__atomic_load_n(&self->state, __ATOMIC_ACQUIRE) != 3) {
        if (stream->stopping && !(stream->flags & 4))
            fn_7100221d48(stream);
        SleepFor_b68a1(stream->interval);
    }
}
