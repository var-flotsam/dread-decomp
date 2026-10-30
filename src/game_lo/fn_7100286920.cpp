// In the navigation query's unit (unit 357 of tools/tu_map.py; src/game/CAIComponent.cpp lays the
// query out): sets the query's start point, cell and side (+0x70); the mode at +0x7c becomes 1 for
// no side (0xff) and 2 with a cell, and is left as it was without either. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavQuery {
    u8 pad_0x0[0x70];
    CVector2D start;  // 0x70
    u16 cell;         // 0x78
    u8 side;          // 0x7a
    u8 pad_0x7b;
    u32 mode;         // 0x7c
};

void fn_7100286920(UnkNavQuery* self, const CVector2D* point, u16 cell, u8 side) {
    self->start.x = point->x;
    self->start.y = point->y;
    self->cell = cell;
    self->side = side;
    u32 mode;
    if (side == 0xff) {
        mode = 1;
    } else {
        if (cell == 0xffff)
            return;
        mode = 2;
    }
    self->mode = mode;
}
