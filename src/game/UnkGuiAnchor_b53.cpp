// From unit 2755 of tools/tu_map.py (0x71011f6044-0x71011f688c), as src/game/UnkGuiAnchor.cpp: the four
// corners of a GUI item. Its types are repeated here as they are defined there. Names and layouts are
// placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

// From src/game/UnkGuiAnchor.cpp (the same definitions).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
    UnkGuiPos& operator=(const UnkGuiPos& other) = default;
};

struct UnkGuiAnchor {
    u32 horizontal;
    u32 vertical;
    UnkGuiPos offset;
};

struct UnkGuiItemState {
    UnkGuiAnchor anchor;
    f32 angle;
    f32 width;
    f32 height;
    f32 scale_x;
    f32 scale_y;
};

// The screen's two aspect factors.
f32 fn_7100132ff0();
f32 fn_7100132fe0();

// As fn_71011f61bc: a position scaled by the state's scale and rotated by its angle.
static inline UnkGuiPos Transform_b53(const UnkGuiItemState* state, const UnkGuiPos& pos) {
    UnkGuiPos r(pos);
    r.x = state->scale_x * r.x;
    r.y = state->scale_y * r.y;
    if (state->angle != 0.0f) {
        r.y = fn_7100132ff0() * r.y;
        UnkVec2Result t = fn_710007a2c0(*reinterpret_cast<const UnkVec2Result*>(&r), state->angle);
        r.x = t.x;
        r.y = t.y;
        r.y = fn_7100132fe0() * r.y;
    }
    return r;
}

// The item's four corners (top left, top right, bottom right, bottom left), placed at its position.
void fn_71011f6624(const UnkGuiItemState* state, UnkGuiPos* corners) {
    f32 left = 0.0f;
    f32 top = 0.0f;
    f32 right = left + state->width;
    f32 bottom = top + state->height;
    UnkGuiPos points[4] = {UnkGuiPos(left, top), UnkGuiPos(right, top), UnkGuiPos(right, bottom),
                           UnkGuiPos(left, bottom)};
    for (int i = 0; i < 4; ++i) {
        UnkGuiPos p = Transform_b53(state, points[i]);
        const UnkGuiPos& o = state->anchor.offset;
        corners[i] = UnkGuiPos(p.x + o.x, p.y + o.y);
    }
}

