// GUI::CLabel: positions the text lines of the label's selected page from the display state.
// Names and layouts are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "types.h"

namespace {

struct UnkLineData {
    u8 pad_0x0[0x1c];
    f32 field_0x1c;
};

struct UnkLineBlock {
    u8 pad_0x0[0x28];
    f32 field_0x28;
    u8 pad_0x2c[0x8];
    f32 field_0x34;
    f32 field_0x38;
    u8 pad_0x3c[0x4];
    u8* field_0x40;  // entries of 0x30 bytes
    u32 field_0x48;
};

struct UnkLineNode {
    UnkLineBlock* block;
    UnkLineNode* next;
};

struct UnkPage {
    u8 pad_0x0[0x4];
    f32 field_0x4;
    u8 pad_0x8[0x10];
    UnkLineNode* lines;
};

struct UnkPageNode {
    UnkPage* page;
    UnkPageNode* next;
};

struct UnkDisplayMetrics {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
};

struct UnkLabel : UnkGuiDisplayObject {
    u8 pad_0x198[0x11c];
    u32 field_0x2b4;
    u8 pad_0x2b8[0x10];
    u32 field_0x2c8;
    u8 pad_0x2cc[0x4];
    UnkPageNode* field_0x2d0;
    u8 pad_0x2d8[0x18];
    u32 field_0x2f0;
    u8 pad_0x2f4[0x30];
    f32 field_0x324;
    u32 field_0x328;
};

}  // namespace

void fn_71011eec70(UnkGuiDisplayObject* object) {
    UnkLabel* self = static_cast<UnkLabel*>(object);
    UnkDisplayMetrics* m = reinterpret_cast<UnkDisplayMetrics*>(fn_71011cde14(self));

    u32 index = self->field_0x2f0;
    u32 count = self->field_0x2c8;
    if (count <= index)
        return;
    UnkPageNode* node = self->field_0x2d0;
    while (index != 0) {
        node = node->next;
        count--;
        index--;
        if (count == 0)
            return;
    }
    if (!node)
        return;
    UnkLineNode* line = node->page->lines;
    if (!line)
        return;
    for (;;) {
        UnkPage* page = node->page;
        UnkLineBlock* b = line->block;
        f32 w = m->field_0x20 * (m->field_0x18 - page->field_0x4);
        if (w > 0.0f) {
            if (self->field_0x328 == 1) {
                b->field_0x38 = w * 0.5f;
            } else if (self->field_0x328 == 2) {
                b->field_0x38 = w;
            }
        }
        f32 y = 0.0f;
        switch (self->field_0x2b4) {
        case 0:
            y = self->field_0x324;
            break;
        case 1:
            y = m->field_0x1c * (m->field_0x14 - b->field_0x28) * 0.5f;
            break;
        case 2:
            y = m->field_0x1c * (m->field_0x14 - b->field_0x28) - self->field_0x324;
            break;
        }
        b->field_0x34 = y;
        if (b->field_0x48 != 0) {
            f32 v = y / m->field_0x1c;
            for (u64 i = 0; i < b->field_0x48; i++)
                __builtin_memcpy(b->field_0x40 + i * 0x30 + 0x1c, &v, sizeof(v));
        }
        line = line->next;
        if (!line)
            return;
    }
}
