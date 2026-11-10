// Unit 20: the matrix of a transform animated by nine channels (translation, angles, scale): each channel
// is evaluated at time t by one of two functions picked by its bit of the flags (+0x8), given the
// channel set at +0xc and the channel's index. Names and layouts are placeholders.
#include <new>

#include "game/UnkXformNode.h"
#include "types.h"

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);

extern f32 (*g_unk_7101a19088[2])(void* channels, u32 index, f32 t);

struct UnkChannelSet_ovb0262p {
    u8 pad_0x0[8];
    u32 flags;  // 0x8
    u8 channels[1];  // 0xc
};

static inline f32 Channel(UnkChannelSet_ovb0262p* self, u32 index, f32 t) {
    return g_unk_7101a19088[(self->flags >> index) & 1](self->channels, index, t);
}

struct UnkAnimTrack_b67a1;

void fn_7100012b60(UnkMatrix44* out, const UnkAnimTrack_b67a1* track, f32 t) {
    UnkChannelSet_ovb0262p* self = const_cast<UnkChannelSet_ovb0262p*>(reinterpret_cast<const UnkChannelSet_ovb0262p*>(track));
    UnkMatrix44 m;
    UnkVec3f translation(Channel(self, 0, t), Channel(self, 1, t), Channel(self, 2, t));
    UnkVec3f angle(Channel(self, 3, t), Channel(self, 4, t), Channel(self, 5, t));
    UnkVec3f scale(Channel(self, 6, t), Channel(self, 7, t), Channel(self, 8, t));
    new (&m) UnkMatrix44(fn_71000791a8(translation, angle, scale));
    UnkCopyMatrix(out, m);
}
