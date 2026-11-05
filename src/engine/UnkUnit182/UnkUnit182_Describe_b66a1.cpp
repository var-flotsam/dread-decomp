// tools/tu_map.py unit 182 (no class hint): a render target's description filled in by its format;
// each format kind has its own filler. Names are placeholders.
#include "types.h"

struct UnkRenderTargetSlot;

void fn_710015f880(UnkRenderTargetSlot* out, u32 width, u32 a, void* b, void* c, u32 d);
void fn_710015fd50(UnkRenderTargetSlot* out, u32 width, u32 a, void* b, void* c, u32 d);
void fn_710015fff0(UnkRenderTargetSlot* out, u32 width, u32 height);
void fn_7100160260(UnkRenderTargetSlot* out, u32 format, u32 width, u32 a, void* b, void* b2, u32 d);

void fn_710015f810(UnkRenderTargetSlot* out, u32 format, u32 width, u32 height, u32 a, void* b, void* c, u32 d,
                   u32 e) {
    switch (format) {
    case 2:
        fn_710015f880(out, width, a, b, c, d);
        return;
    case 6:
        fn_710015fff0(out, width, height);
        return;
    case 5:
        fn_710015fd50(out, width, a, b, c, d);
        return;
    default:
        fn_7100160260(out, format, width, a, b, b, d);
        return;
    }
}
