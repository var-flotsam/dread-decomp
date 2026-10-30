// Translation unit 1672 of data/tu_map.csv (0x7100b1c56c-0x7100b1c73c, no class hint): a GUI
// item's display state (0x40 bytes: its anchoring, angle, size, scale, depth, colour and flags)
// composed with its parent's (fn_7100b1c56c). The anchoring is the one of src/game/UnkGuiAnchor.cpp,
// whose types are repeated here as they are defined there. Names and layouts are placeholders.
#include "types.h"

// From src/game/UnkGuiAnchor.cpp (the same definitions).
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

struct UnkGuiArea {
    u8 pad_0x0[0x14];
    f32 width;
    f32 height;
};

struct UnkGuiColor {
    f32 r, g, b, a;
};

struct UnkGuiScale {
    f32 x, y;

    UnkGuiScale operator*(const UnkGuiScale& o) const { return {x * o.x, y * o.y}; }
};

struct UnkGuiItemState {
    UnkGuiAnchor anchor;  // the offset is the position
    f32 angle;
    f32 width;   // the area a child is anchored in (UnkGuiArea)
    f32 height;
    UnkGuiScale scale;
    f32 depth;
    UnkGuiColor color;
    s32 field_0x38;  // INT32_MAX: the parent's
    bool field_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
};
static_assert(sizeof(UnkGuiItemState) == 0x40, "UnkGuiItemState placeholder layout");

// The anchored position of an item inside an area (src/game/UnkGuiAnchor.cpp).
UnkGuiPos fn_71011f6044(const UnkGuiAnchor* self, const UnkGuiArea* area);
// A position scaled and rotated by a state's scale and angle.
UnkGuiPos fn_71011f61bc(const UnkGuiItemState* state, const UnkGuiPos* pos);
// The component-wise product of two colours.
UnkGuiColor fn_71011c1740(const UnkGuiColor* a, const UnkGuiColor* b);

// A flag set in both, as a byte.
static inline u8 UnkBothSet(u8 a, u8 b) { return a ? b : 0; }

// Copies a position component by component.
static inline void UnkSetPos(UnkGuiPos* dst, const UnkGuiPos& src) {
    dst->x = src.x;
    dst->y = src.y;
}

// The state of `child` under `parent` into `out` (which may be either): the byte at 0x3e from the
// parent, the flags at 0x3c and 0x3d where both have them, the s32 at 0x38 from the child unless it
// is INT32_MAX, colours and scales multiplied, angles and depths added, the size the child's. With
// bit 0 of `changes` (the child object's change bits, its +0x144) set, the child's anchored position
// inside the parent (scaled and rotated by the parent when `mode` has 0x2 or 0x10) is offset by the
// parent's, and the result is no longer anchored.
void fn_7100b1c56c(const UnkGuiItemState* parent, UnkGuiItemState* out, const UnkGuiItemState* child, u32 mode,
                   u32 changes) {
    out->field_0x3e = parent->field_0x3e;
    out->field_0x3c = parent->field_0x3c && child->field_0x3c;
    out->field_0x3d = UnkBothSet(parent->field_0x3d, child->field_0x3d);
    out->field_0x38 = child->field_0x38 == 0x7fffffff ? parent->field_0x38 : child->field_0x38;
    out->angle = child->angle + parent->angle;
    out->depth = child->depth + parent->depth;
    out->color = fn_71011c1740(&child->color, &parent->color);
    out->width = child->width;
    out->height = child->height;
    out->scale = child->scale * parent->scale;
    if (changes & 1) {
        out->anchor.horizontal = 0;
        out->anchor.vertical = 0;
        UnkSetPos(&out->anchor.offset, fn_71011f6044(&child->anchor, reinterpret_cast<const UnkGuiArea*>(parent)));
        if (mode & 0x12)
            UnkSetPos(&out->anchor.offset, fn_71011f61bc(parent, &out->anchor.offset));
        out->anchor.offset.x = parent->anchor.offset.x + out->anchor.offset.x;
        out->anchor.offset.y = parent->anchor.offset.y + out->anchor.offset.y;
    }
}
