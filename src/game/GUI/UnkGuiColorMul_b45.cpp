// From unit 2746 of tools/tu_map.py: the component-wise product of two GUI colours (returned in
// s0-s3), as src/game/UnkGuiItemState.cpp declares and uses it. Names are placeholders.
#include "types.h"

// As src/game/UnkGuiItemState.cpp defines it.
struct UnkGuiColor {
    f32 r, g, b, a;
};

UnkGuiColor fn_71011c1740(const UnkGuiColor* a, const UnkGuiColor* b) {
    return {a->r * b->r, a->g * b->g, a->b * b->b, a->a * b->a};
}
