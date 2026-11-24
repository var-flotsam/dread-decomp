// Unit 259: draws one render command: checks its render item against the system's mask and pass,
// binds the item (or counts a repeat of the bound one), and draws the command's data, inside a
// pushed blend state when the pass asks for one. Names and layouts are placeholders.
#include "game/UnkBlendStack.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkTU177.h"
#include "game/UnkTU180.h"
#include "game/UnkTU230.h"
#include "game/UnkTU259.h"
#include "types.h"

struct UnkRenderItem_l1d0638 {
    u8 pad_0x0[0x18];
    u32 mask;  // 0x18
    u8 pad_0x1c[0x48 - 0x1c];
    u32 field_0x48;
    u8 pad_0x4c[0x80 - 0x4c];
    u8 field_0x80;
    u8 pad_0x81[0xb0 - 0x81];
    u8 field_0xb0;
};

struct UnkGfxItem_b0155;
struct UnkRenderCmd_l1d0638 {
    UnkGfxItem_b0155* data;
    UnkRenderItem_l1d0638* item;
};

struct UnkGfx_l1d0638 {
    u8 pad_0x0[0x317a998];
    u32 mask;  // 0x317a998
    s32 pass;  // 0x317a99c
    u8 pad_0x317a9a0[0x317acc0 - 0x317a9a0];
    UnkRenderItem_l1d0638* bound;  // 0x317acc0
    u8 pad_0x317acc8[0x317af68 - 0x317acc8];
    void* state;    // 0x317af68
    void* program;  // 0x317af70
    u32 field_0x317af78;
    u8 pad_0x317af7c[0x318366b - 0x317af7c];
    u8 field_0x318366b;
    u8 pad_0x318366c[0x6];
    u8 field_0x3183672;
};
static_assert(__builtin_offsetof(UnkGfx_l1d0638, field_0x3183672) == 0x3183672, "UnkGfx_l1d0638 layout");

struct UnkRenderCmd_b0247;
bool fn_7100196e80(UnkRenderCmd_b0247* cmd);
struct UnkGfx_l1d010c;
struct UnkDrawItem_l1d010c;
void fn_71001d010c(UnkGfx_l1d010c* self, UnkDrawItem_l1d010c* item, bool flag);
struct UnkGraphicsSystem;
void fn_71001d03f8(UnkGraphicsSystem* self, UnkGfxItem_b0155* item, bool upload, bool draw);
struct UnkFlags_b0237;
bool PushStencilStateAndApply(UnkFlags_b0237* request_);

extern const f32 g_unk_716e589b44[4] __attribute__((visibility("hidden")));

static inline void Draw(UnkGfx_l1d0638* self, UnkGfxItem_b0155* data) {
    fn_71001d03f8(reinterpret_cast<UnkGraphicsSystem*>(self), data, true, true);
}

// Draws the data; inside a pushed blend state when the pass asks for one and the item allows it.
static inline void DrawItem(UnkGfx_l1d0638* self, UnkGfxItem_b0155* data, UnkRenderItem_l1d0638* item) {
    if (item && self->field_0x318366b && !item->field_0xb0 && self->field_0x3183672 && self->field_0x317af78 == 0 &&
        self->mask == 1) {
        UnkBlendState state;
        state.enable = true;
        state.field_0x19 = 0;
        state.field_0x0 = 0;
        state.field_0x4 = 0;
        state.field_0x8 = 2;
        state.mode = 0;
        state.field_0x10 = 3;
        state.field_0x14 = 1;
        PushStencilStateAndApply(reinterpret_cast<UnkFlags_b0237*>(&state));
        Draw(self, data);
        self->bound = nullptr;
        PopStateStackAndApplyNewTop(true);
        return;
    }
    Draw(self, data);
}

void fn_71001d0638(UnkGlobal_710b372a98* gfx, void* item_, void* a, u32 b) {
    if (!item_)
        return;
    UnkGfx_l1d0638* self = reinterpret_cast<UnkGfx_l1d0638*>(gfx);
    UnkRenderCmd_l1d0638* cmd = static_cast<UnkRenderCmd_l1d0638*>(item_);
    if (!fn_7100196e80(reinterpret_cast<UnkRenderCmd_b0247*>(cmd)))
        return;
    UnkGfxItem_b0155* data = cmd->data;
    if (!data)
        return;
    if (a) {
        DrawItem(self, data, static_cast<UnkRenderItem_l1d0638*>(a));
        return;
    }
    UnkRenderItem_l1d0638* item = cmd->item;
    if (!item)
        return;
    if (!(item->mask & self->mask))
        return;
    if (self->pass == 2) {
        if (item->field_0x80)
            return;
        fn_710015ab00(item->field_0x48);
        item = cmd->item;
    }
    if (self->pass == 7 && !item->field_0xb0)
        return;
    if (self->field_0x318366b) {
        if (self->bound == item)
            ++g_unk_716e554938;
        else
            // The flag is the low bit of b.
            fn_71001d010c(reinterpret_cast<UnkGfx_l1d010c*>(self), reinterpret_cast<UnkDrawItem_l1d010c*>(item),
                          b & 1);
    } else if (self->pass == 12 && self->program) {
        u64 location;
        if (fn_71001aa0b4(self->program, 1, 0x119, &location))
            fn_710015e1f0(self->state, 1, location, 4, g_unk_716e589b44);
    }
    DrawItem(self, data, item);
}
