// Unit 182 of tools/tu_map.py (no reflected class): a buffer's size (+0x1c, 0 without a buffer;
// used by src/engine/UnkUnit252/UnkUnit252_Upload.cpp) and whether bit `index` of the flags at
// +0x28 is set (used by src/game_lo/fn_710019ab0c.cpp). Names and layouts are placeholders.
#include "game/UnkTU182.h"
#include "types.h"

struct UnkGfxBuffer252;
struct UnkSource_710019ab0c;

struct UnkUnit182Buffer_b65a1 {
    u8 pad_0x0[0x1c];
    u32 size;
};

struct UnkUnit182Source_b65a1 {
    u8 pad_0x0[0x28];
    u32 flags;
};

u32 fn_71001622e0(UnkGfxBuffer252* buffer) {
    UnkUnit182Buffer_b65a1* self = reinterpret_cast<UnkUnit182Buffer_b65a1*>(buffer);
    return self ? self->size : 0;
}

bool fn_7100161fb0(UnkSource_710019ab0c* source, s32 index) {
    UnkUnit182Source_b65a1* self = reinterpret_cast<UnkUnit182Source_b65a1*>(source);
    return self ? (self->flags & (1u << index)) != 0 : false;
}
