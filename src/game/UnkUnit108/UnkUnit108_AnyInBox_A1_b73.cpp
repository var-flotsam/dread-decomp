// From unit 108 of tools/tu_map.py (no class hint): whether any point of a list of 12-byte entries (x, y at
// +0, +4) lies in the square of half-size `r` around `p`. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit108Point_A1_b73 {
    f32 x, y;
};

struct UnkUnit108Entry_A1_b73 {
    f32 x, y, z;
};

struct UnkUnit108List_A1_b73 {
    UnkUnit108Entry_A1_b73* data;
    u32 count;
};

bool fn_7100118b1c(const UnkUnit108List_A1_b73* self, const UnkUnit108Point_A1_b73* p, f32 r) {
    const UnkUnit108Entry_A1_b73* e = self->data;
    const UnkUnit108Entry_A1_b73* end = e + self->count;
    f32 min_x = p->x - r;
    f32 max_x = p->x + r;
    f32 min_y = p->y - r;
    f32 max_y = p->y + r;
    for (; e != end; ++e) {
        if (max_x >= e->x && min_x <= e->x && max_y >= e->y && min_y <= e->y)
            return true;
    }
    return false;
}
