// CCharClassToStateTrack (unit 2476 of tools/tu_map.py, the name from reflection): fills a
// transition description from the track. The target state's hash (+0x18), then either one of two
// global blend settings (by the mode at +0x24, when +0x20 is set) or none with the frame count
// (+0x28) as a float; flag 2 when +0x2c is set, and the duration (+0x30) when it is not negative.
// Names and layouts are placeholders.
#include "game/UnkTU1730.h"
#include "types.h"

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

// A float with a flag saying it is set (the float's bits in the low word, the flag in the high).
struct UnkTrackOptional {
    u64 bits;
};

struct UnkTrackStateName {
    u8 unk_0x0[0x30];
    u64 hash;
};

struct UnkTrackTiming {
    s64 id;
    UnkTrackOptional duration;
};

struct UnkToStateTrack {
    u8 unk_0x0[0x18];
    UnkTrackStateName* state;
    bool has_blend;
    u8 pad_0x21[0x3];
    s32 blend_mode;
    s32 frames;
    bool flag;
    u8 pad_0x2d[0x3];
    f32 duration;
};

struct UnkTrackTransition {
    u64 state;
    UnkTrackTiming timing;
    UnkTrackBlend blend;
    u32 flags;
};

void fn_7100d2a9bc(UnkToStateTrack* self, UnkTrackTransition* out) {
    out->state = self->state->hash;
    if (self->has_blend) {
        const UnkTrackBlend* blend = self->blend_mode == 1 ? &g_unk_7172769758 : &g_unk_7172769770;
        out->blend = *blend;
    } else {
        s32 frames = self->frames;
        out->blend.kind = 0;
        out->blend.value = (f32)frames;
    }
    if (self->flag)
        out->flags |= 2;
    f32 duration = self->duration;
    if (duration >= 0.0f) {
        u32 value;
        __builtin_memcpy(&value, &duration, sizeof(value));
        out->timing = {-1, {(u64)value | (1ull << 32)}};
    }
}
