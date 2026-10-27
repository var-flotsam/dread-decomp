// From unit 2755 of tools/tu_map.py (0x71011f6044-0x71011f688c, no class hint; the next unit is
// GUI::CSprite's): a GUI item's position from its anchoring. Each axis has a mode (0 keeps the
// offset; 1 centres in the area; 2 and 3 subtract one of the item's two extents, from twice the
// offset and from the area's size) and an offset; the extents come from fn_71011f638c. Names and
// layouts are placeholders.
// Holds fn_71011f6044.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// A 2D position returned in memory (its user-provided copy keeps it out of registers).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

struct UnkGuiAnchor {
    u32 horizontal;
    u32 vertical;
    UnkGuiPos offset;
};

// The area the item is placed in: its size at +0x14.
struct UnkGuiArea {
    u8 pad_0x0[0x14];
    f32 width;
    f32 height;
};

// The item's two extents.
void fn_71011f638c(const UnkGuiAnchor* self, CVector2D* a, CVector2D* b);

// The anchored position inside `area` (the offsets as they are when neither axis is anchored).
UnkGuiPos fn_71011f6044(const UnkGuiAnchor* self, const UnkGuiArea* area) {
    UnkGuiPos pos(self->offset);
    if (self->horizontal == 0 && self->vertical == 0)
        return pos;
    CVector2D a = {0.0f, 0.0f};
    CVector2D b = {0.0f, 0.0f};
    fn_71011f638c(self, &a, &b);
    switch (self->horizontal) {
    case 0:
        pos.x = self->offset.x;
        break;
    case 1:
        pos.x = (self->offset.x + self->offset.x + area->width * 0.5f) - (b.x + a.x) * 0.5f;
        break;
    case 2:
        pos.x = (self->offset.x + self->offset.x) - a.x;
        break;
    case 3:
        pos.x = area->width - b.x;
        break;
    }
    switch (self->vertical) {
    case 0:
        pos.y = self->offset.y;
        break;
    case 1:
        pos.y = (self->offset.y + self->offset.y + area->height * 0.5f) - (b.y + a.y) * 0.5f;
        break;
    case 2:
        pos.y = (self->offset.y + self->offset.y) - a.y;
        break;
    case 3:
        pos.y = area->height - b.y;
        break;
    }
    return pos;
}
