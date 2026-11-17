// Unit 263's static initialiser: the two header string ids every unit copies ("binaries",
// "sources", after a u32 6), the minimap render pass ids and the "MINIMAP" id, each registered
// for destruction. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrIdBase;

struct UnkStatics_716e596638 {
    bool pending;  // a region waits to be uploaded
    u8 pad_0x1[0x3];
    u32 field_0x4;
    u32 x_begin;  // 0x8
    u32 x_end;
    u32 y_begin;
    u32 y_end;
    f32 field_0x18[3];
    u8 pad_0x24[0x4];
    UnkStrIdBase binaries;
    UnkStrIdBase sources;
    UnkStrIdBase minimap_pass;
    UnkStrIdBase minimap_mask_pass;
    UnkStrIdBase minimap_zone_pass;
    UnkStrIdBase minimap_color_pass;
    UnkStrIdBase minimap_maproom_pass;
    UnkStrIdBase minimap_2d_pass;
    UnkStrIdBase minimap_filter_pass;
    UnkStrIdBase minimap;
    u32* pixels;  // 0x78
};
static_assert(__builtin_offsetof(UnkStatics_716e596638, pixels) == 0x78, "UnkStatics_716e596638 layout");
extern UnkStatics_716e596638 g_unk_716e596638 __attribute__((visibility("hidden")));

static inline void InitId(UnkStrIdBase* id, const char* str) {
    new (id) UnkStrIdBase(str, true);
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

void fn_71001e50b0() {
    UnkStatics_716e596638* s = &g_unk_716e596638;
    s->field_0x4 = 6;
    InitId(&s->binaries, "binaries");
    InitId(&s->sources, "sources");
    InitId(&s->minimap_pass, "MINIMAP_PASS");
    InitId(&s->minimap_mask_pass, "MINIMAP_MASK_PASS");
    InitId(&s->minimap_zone_pass, "MINIMAP_ZONE_PASS");
    InitId(&s->minimap_color_pass, "MINIMAP_COLOR_PASS");
    InitId(&s->minimap_maproom_pass, "MINIMAP_MAPROOM_PASS");
    InitId(&s->minimap_2d_pass, "MINIMAP_2D_PASS");
    InitId(&s->minimap_filter_pass, "MINIMAP_FILTER_PASS");
    InitId(&s->minimap, "MINIMAP");
}
