// Unit 109 of tools/tu_map.py (after unit 108's 2D polygons, src/base/spatial/UnkPolygon2D.cpp): applies
// fn_7100119fcc with a point and an empty filter to every 0x48-byte polygon of a list. Names are
// placeholders.
#include <functional>

#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygon2D_A1b71 {
    u8 unk_0x0[0x48];
};

struct UnkPolygonList_A1b71 {
    UnkPolygon2D_A1b71* data;
    u32 count;
};

void fn_7100119fcc(UnkPolygon2D_A1b71* polygon, const CVector2D* point,
                   const std::function<bool(const CVector2D&)>& filter);

void fn_710012524c(UnkPolygonList_A1b71* self, const CVector2D* point) {
    for (UnkPolygon2D_A1b71* it = self->data, *end = it + self->count; it != end; ++it)
        fn_7100119fcc(it, point, std::function<bool(const CVector2D&)>());
}
