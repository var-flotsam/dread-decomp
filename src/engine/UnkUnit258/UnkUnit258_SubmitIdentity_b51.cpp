// Engine unit 258: like fn_71001c4720 (src/engine/UnkUnit258/UnkUnit258.cpp), but stores the
// identity matrix (g_unk_7101a1a680) in the graphics system and hands over its single item or,
// without one, the non-null items of a pointer list, first to last, with the last argument set.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

void fn_71001d0638(UnkGlobal_710b372a98* gfx, void* item, void* a, u32 b);
extern UnkMatrix44 g_unk_7101a1a680;

struct UnkGfxItemPtrList {
    u8 pad_0x0[0x28];
    u32 count;
    u8 pad_0x2c[0x44];
    void** items;
};

struct UnkGfxSubmitIdentity {
    u8 pad_0x0[0x8];
    UnkGfxItemPtrList* list;
    u8 pad_0x10[0x8];
    void* single;
};

void fn_71001c47c0(UnkGfxSubmitIdentity* self) {
    UnkGlobal_710b372a98* gfx = g_unk_710b372a98;
    UnkGfxItemPtrList* list = self->list;
    void* single = self->single;
    *reinterpret_cast<const UnkMatrix44**>(gfx->pad_0x0 + 0x317ab28) = &g_unk_7101a1a680;
    if (single) {
        fn_71001d0638(gfx, single, nullptr, 1);
        return;
    }
    u32 n = list->count;
    for (u32 i = 0; i < n; ++i)
        if (void* item = list->items[i])
            fn_71001d0638(gfx, item, nullptr, 1);
}
