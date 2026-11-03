// Unit 182 of tools/tu_map.py: writes one texel of a texture through an NVN entry point loaded at
// start-up (a texture-write callback taking a view, a region and the data). Names are placeholders.
#include "types.h"

struct UnkTexRegion {
    s32 x, y, z;
    s32 width, height, depth;
};

struct UnkTexelTarget {
    u32 width;
    u32 height;
    u8 pad_0x8[0x10];
    u8 texture[1];  // 0x18
};

extern void (*g_unk_716e29b320)(void* texture, void* view, const UnkTexRegion* region, const void* data);

void fn_7100160e80(UnkTexelTarget* target, u32 x, u32 y, u32 value) {
    if (target && target->width >= x && target->height >= y) {
        UnkTexRegion region;
        region.x = x;
        region.y = y;
        region.height = 1;
        region.depth = 1;
        region.z = 0;
        region.width = 1;
        asm("" ::: "memory");  // the original builds the region before loading the entry point
        g_unk_716e29b320(target->texture, nullptr, &region, &value);
    }
}
