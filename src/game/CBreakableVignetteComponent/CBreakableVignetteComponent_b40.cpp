// CBreakableVignetteComponent unit (tools/tu_map.py unit 1168): fn_7100763318 asks the render manager to
// show or hide the vignette's object (fn_7100219c6c) until it no longer answers 2, tells the map
// (fn_7100e8d8b0, fn_7100e95130) about it when shown and flagged (+0x3c), then moves the callback link at
// +0x48 to the manager g_unk_7101d47360 (created on first use; its list at +0xa60).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU315.h"
#include "game/UnkTU883.h"
#include "types.h"

struct UnkMapData;
struct UnkCallback;
void fn_7100e95130(UnkMapData* map, u64 id);
struct UnkManager;

struct UnkKeyed_bv {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBreakableVignette_bv {
    u8 pad_0x0[0x30];
    u64 field_0x30;
    u8 pad_0x38[1];
    bool field_0x39;
    u8 pad_0x3a[2];
    bool field_0x3c;
    u8 pad_0x3d[3];
    UnkKeyed_bv* field_0x40;
    void* field_0x48;
};

void fn_7100763318(UnkBreakableVignette_bv* self) {
    auto show = reinterpret_cast<u32 (*)(UnkRenderManager*, bool, u64, u32, bool)>(fn_7100219c6c);
    while (show(g_unk_7101d47300, !self->field_0x39, self->field_0x40->key, 0, false) == 2) {
    }
    if (!self->field_0x39 && self->field_0x3c)
        fn_7100e95130(fn_7100e8d8b0(), self->field_0x30);
    void** link = &self->field_0x48;
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    if (*link) {
        fn_71005c4828(*link, link);
        *link = nullptr;
    }
    void* list = static_cast<char*>(manager) + 0xa60;
    *link = list;
    fn_71005c41bc(list, reinterpret_cast<UnkCallback*>(link));
}
