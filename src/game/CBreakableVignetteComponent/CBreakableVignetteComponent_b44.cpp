// CBreakableVignetteComponent unit (tools/tu_map.py unit 1168): asks the render manager to show or
// hide the vignette's object (fn_7100219c6c) until it no longer answers 2, as fn_7100763318 in
// CBreakableVignetteComponent_b40.cpp does first. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

void fn_7100219c6c(UnkRenderManager*, bool, u64, u32, bool);

struct UnkKeyed_b44a2 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBreakableVignette_b44a2 {
    u8 pad_0x0[0x39];
    bool field_0x39;
    u8 pad_0x3a[6];
    UnkKeyed_b44a2* field_0x40;
};

u32 fn_71007636c8(UnkBreakableVignette_b44a2* self) {
    auto show = reinterpret_cast<u32 (*)(UnkRenderManager*, bool, u64, u32, bool)>(fn_7100219c6c);
    while (show(g_unk_7101d47300, !self->field_0x39, self->field_0x40->key, 0, false) == 2) {
    }
    return 0;
}
