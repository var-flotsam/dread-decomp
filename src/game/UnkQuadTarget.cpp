// Translation unit 223 of data/tu_map.csv (0x710019bd2c-0x710019c69c, no class hint): an object
// with four 2D corner values at +0xc8 (in the order corner 2, 0, 1, 3) and a dirty flag at +0x180;
// its unit also sets a matrix (fn_710019c474) and four floats (fn_710019c564) on it, and holds the
// destructors in src/game_s1/fn_710019bfb8.cpp.
// Holds fn_710019c124 (sets a corner by index). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkQuadTarget {
    u8 unk_0x0[0xc8];
    CVector2D corner_2;
    CVector2D corner_0;
    CVector2D corner_1;
    CVector2D corner_3;
    u8 unk_0xe8[0x98];
    bool dirty;
};
static_assert(__builtin_offsetof(UnkQuadTarget, corner_2) == 0xc8, "UnkQuadTarget placeholder layout");
static_assert(__builtin_offsetof(UnkQuadTarget, dirty) == 0x180, "UnkQuadTarget placeholder layout");

static inline void Assign(CVector2D& dst, const CVector2D& src) {
    dst.x = src.x;
    dst.y = src.y;
}

// Sets corner `index` (0 to 3; nothing for any other) and marks the object dirty either way.
void fn_710019c124(UnkQuadTarget* self, u32 index, const CVector2D& value) {
    switch (index) {
    case 0:
        Assign(self->corner_0, value);
        break;
    case 1:
        Assign(self->corner_1, value);
        break;
    case 2:
        Assign(self->corner_2, value);
        break;
    case 3:
        Assign(self->corner_3, value);
        break;
    }
    self->dirty = true;
}
