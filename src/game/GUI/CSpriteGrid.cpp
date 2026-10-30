// GUI::CSpriteGrid's unit (unit 2757 of tools/tu_map.py, the name from reflection): the constructor,
// over the display object's (fn_71011cce1c, game/UnkGuiDisplayObject.h). Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkGuiDisplayObject.h"

using base::global::CFilePathStrId;

extern char g_unk_7101cb6ac8[];  // CSpriteGrid's vtable
void fn_71011c1728(void* color);  // a white colour (four ones)

struct UnkGuiSpriteGrid {
    char* vtable;
    u8 base_0x8[0x181];
    bool field_0x189;
    u8 pad_0x18a[0x6];
    u64 field_0x190;
    CFilePathStrId path;  // 0x198: the texture's
    u64 field_0x1a0;
    u64 field_0x1a8;
    u8 color[0x10];  // 0x1b0
    u64 field_0x1c0;
    u64 field_0x1c8;
    u64 field_0x1d0;
    u64 field_0x1d8;
    u8 field_0x1e0[0x3];  // cleared with the four words before it
};
static_assert(__builtin_offsetof(UnkGuiSpriteGrid, field_0x1d8) == 0x1d8, "UnkGuiSpriteGrid layout");

void fn_71011fca70(UnkGuiSpriteGrid* self) {
    fn_71011cce1c(reinterpret_cast<UnkGuiDisplayObject*>(self));
    self->vtable = g_unk_7101cb6ac8 + 0x10;
    self->field_0x189 = false;
    self->field_0x190 = 0;
    self->path.data = nullptr;
    // The empty path, from the pool (made on first use).
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->path.data = &g_unk_7101d47200->empty;
    self->field_0x1a0 = 0;
    self->field_0x1a8 = 0;
    fn_71011c1728(self->color);
    __builtin_memset(&self->field_0x1c0, 0, 0x23);
}
