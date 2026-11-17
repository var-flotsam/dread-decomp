#pragma once

// The statics block of unit 263 (the minimap render passes): a pending texel upload (the flag, the rectangle to
// write and the pixels, 1024 per row) and the pass string ids its static initialiser fn_71001e50b0 builds. Names
// and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkMinimapRenderStatics {
    u8 pending;  // 0x0: a region waits to be uploaded
    u8 pad_0x1[0x3];
    u32 field_0x4;
    u32 x_begin;  // 0x8
    u32 x_end;  // 0xc
    u32 y_begin;  // 0x10
    u32 y_end;  // 0x14
    f32 field_0x18[3];
    u8 pad_0x24[0x4];
    base::global::UnkStrIdBase binaries;  // 0x28
    base::global::UnkStrIdBase sources;
    base::global::UnkStrIdBase minimap_pass;
    base::global::UnkStrIdBase minimap_mask_pass;
    base::global::UnkStrIdBase minimap_zone_pass;
    base::global::UnkStrIdBase minimap_color_pass;
    base::global::UnkStrIdBase minimap_maproom_pass;
    base::global::UnkStrIdBase minimap_2d_pass;
    base::global::UnkStrIdBase minimap_filter_pass;
    base::global::UnkStrIdBase minimap;
    u32* pixels;  // 0x78
};
static_assert(__builtin_offsetof(UnkMinimapRenderStatics, pixels) == 0x78, "UnkMinimapRenderStatics layout");

extern UnkMinimapRenderStatics g_unk_716e596638 __attribute__((visibility("hidden")));
