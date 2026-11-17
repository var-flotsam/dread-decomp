// Unit 263's static initialiser: the two header string ids every unit copies ("binaries",
// "sources", after a u32 6), the minimap render pass ids and the "MINIMAP" id, each registered
// for destruction. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkMinimapRenderStatics.h"
#include "types.h"

using base::global::UnkStrIdBase;

static inline void InitId(UnkStrIdBase* id, const char* str) {
    new (id) UnkStrIdBase(str, true);
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

void fn_71001e50b0() {
    UnkMinimapRenderStatics* s = &g_unk_716e596638;
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
