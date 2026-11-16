// From unit 2755 of tools/tu_map.py (0x71011f6044-0x71011f688c), as src/game/UnkGuiAnchor.cpp: a GUI item's
// extents, the smallest and largest corner. A rotated item takes them from its four corners; otherwise they
// are its scaled origin and that plus its scaled size. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// From src/game/UnkGuiAnchor_b53.cpp (the same definitions).
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

void fn_71011f6624(const UnkGuiItemState* state, UnkGuiPos* corners);

// Widens [lo, hi] to take in `v`.
static inline void Extend(f32 v, f32& lo, f32& hi) {
    if (v > hi)
        hi = v;
    if (v < lo)
        lo = v;
}

// The item's extents: `min` gets its smallest corner and `max` its largest.
void fn_71011f638c(const UnkGuiAnchor* self, CVector2D* min, CVector2D* max) {
    const UnkGuiItemState* state = reinterpret_cast<const UnkGuiItemState*>(self);
    if (state->angle != 0.0f) {
        UnkGuiPos corners[4] = {UnkGuiPos(0.0f, 0.0f), UnkGuiPos(0.0f, 0.0f), UnkGuiPos(0.0f, 0.0f),
                                UnkGuiPos(0.0f, 0.0f)};
        fn_71011f6624(state, corners);
        min->x = __FLT_MAX__;
        min->y = __FLT_MAX__;
        max->x = -__FLT_MAX__;
        max->y = -__FLT_MAX__;
        for (int i = 0; i < 4; ++i) {
            Extend(corners[i].x, min->x, max->x);
            Extend(corners[i].y, min->y, max->y);
        }
    } else {
        f32 left = 0.0f;
        f32 top = 0.0f;
        CVector2D origin = {state->scale_x * left + state->anchor.offset.x,
                            state->scale_y * top + state->anchor.offset.y};
        *min = origin;
        CVector2D end = {origin.x + state->width * state->scale_x, origin.y + state->height * state->scale_y};
        *max = end;
    }
}
