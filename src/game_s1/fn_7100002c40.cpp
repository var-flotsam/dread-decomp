// Packs a float RGBA colour into four bytes (x lowest): each channel is clamped to [0, 1],
// scaled by 255 and rounded.
#include "types.h"

struct UnkColor4f {
    f32 r, g, b, a;
};

static inline u32 ChannelToByte(f32 v) {
    f32 c = v > 1.0f ? 1.0f : v;
    c = v < 0.0f ? 0.0f : c;
    return (s32)(c * 255.0f + 0.5f);
}

u32 fn_7100002c40(const UnkColor4f* c) {
    return ChannelToByte(c->r) | ChannelToByte(c->g) << 8 | ChannelToByte(c->b) << 16 |
           ChannelToByte(c->a) << 24;
}
