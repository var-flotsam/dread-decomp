// Engine unit 258 (graphics, next to the graphics system behind g_unk_710b372a98). Names and
// layouts are placeholders.
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

void fn_71001d0638(UnkGlobal_710b372a98* gfx, void* item, void* a, u32 b);

struct UnkGfxItem_0x20 {
    u8 pad_0x0[0x20];
};

struct UnkGfxItemList {
    u8 pad_0x0[0x34];
    u32 count;
    UnkGfxItem_0x20* items;
};

struct UnkGfxSubmit {
    u8 pad_0x0[0x8];
    UnkGfxItemList* list;
    u8 pad_0x10[0x8];
    void* single;
    u8 pad_0x20[0x10];
    u64 field_0x30;
};

// Stores the submission's value in the graphics system, then hands it its single item or, without
// one, the list's items from the last to the first.
void fn_71001c4720(UnkGfxSubmit* self) {
    UnkGlobal_710b372a98* gfx = g_unk_710b372a98;
    UnkGfxItemList* list = self->list;
    void* single = self->single;
    typedef u64 __attribute__((may_alias)) AliasU64;
    *reinterpret_cast<AliasU64*>(gfx->pad_0x0 + 0x317ab28) = self->field_0x30;
    if (single) {
        fn_71001d0638(gfx, self->single, nullptr, 0);
        return;
    }
    for (u32 n = list->count; n != 0; --n)
        fn_71001d0638(gfx, &list->items[n - 1], nullptr, 0);
}
