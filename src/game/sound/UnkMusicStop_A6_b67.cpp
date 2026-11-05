// Unit 2627 of tools/tu_map.py and its neighbours (the music request units of
// src/game/sound/UnkMusicPlay_b55.cpp): stops the request through fn_7100fd16ec with the request's value at
// +0x10, unless it is not forced, flagged at +0x30, and fn_7100fd1ad8 takes it first. Four copies.
// Names and layouts are placeholders.
#include "types.h"

// As src/game/sound/UnkMusicPlay_b55.cpp spells it.
struct UnkMusicRequest_b55 {
    u8 unk_0x0[0x10];
    u32 field_0x10;
    u8 unk_0x14[0x1c];
    u8 field_0x30;
};

bool fn_7100fd1ad8(UnkMusicRequest_b55* self, u32 arg);
void fn_7100fd16ec(UnkMusicRequest_b55* self, u32 value, u32 force);

static inline __attribute__((always_inline)) void StopMusic_A6_b67(UnkMusicRequest_b55* self, u32 force) {
    if (__builtin_expect(force == 0, 1)) {
        if (self->field_0x30 && fn_7100fd1ad8(self, 1))
            return;
    }
    fn_7100fd16ec(self, self->field_0x10, force);
}

void fn_7100fce9e0(UnkMusicRequest_b55* self, u32 force) { StopMusic_A6_b67(self, force); }
void fn_7100fcf180(UnkMusicRequest_b55* self, u32 force) { StopMusic_A6_b67(self, force); }
void fn_7100fcf8c0(UnkMusicRequest_b55* self, u32 force) { StopMusic_A6_b67(self, force); }
void fn_7100fcfbd0(UnkMusicRequest_b55* self, u32 force) { StopMusic_A6_b67(self, force); }
