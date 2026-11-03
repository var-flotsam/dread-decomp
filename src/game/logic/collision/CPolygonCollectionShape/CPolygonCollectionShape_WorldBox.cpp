// game::logic::collision::CPolygonCollectionShape (unit 2515 of tools/tu_map.py, the name from
// reflection): the shape's box in the world. When the polygons' bounds are stale (+0xe0) they are
// recomputed first (fn_71001243d4 on the polygon collection at +0xa8); then the 2D bounds
// (+0xd0 min, +0xd8 max) are moved by the transform's translation, with its z for both depths.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPolygonCollection2D;
void fn_71001243d4(UnkPolygonCollection2D* self);

struct UnkWorldVec3 {
    f32 x, y, z;
};

struct UnkWorldBox3D {
    UnkWorldVec3 min;
    UnkWorldVec3 max;
};

struct UnkWorldTransform {
    f32 m[3][4];
};

struct UnkPolygonCollectionBounds {
    void* vtable;
    u8 unk_0x8[0xa0];
    u8 polygons[0x28];  // 0xa8
    f32 min_x, min_y;   // 0xd0
    f32 max_x, max_y;   // 0xd8
    bool stale;         // 0xe0
};

UnkWorldBox3D fn_7100d63490(UnkPolygonCollectionBounds* self, const UnkWorldTransform* transform) {
    if (self->stale)
        fn_71001243d4(reinterpret_cast<UnkPolygonCollection2D*>(self->polygons));
    f32 min_x = self->min_x, min_y = self->min_y;
    f32 max_x = self->max_x, max_y = self->max_y;
    f32 tx = transform->m[0][3];
    f32 ty = transform->m[1][3];
    f32 tz = transform->m[2][3];
    UnkWorldBox3D box;
    box.min = {min_x + tx, min_y + ty, tz + 0.0f};
    box.max = {max_x + tx, max_y + ty, tz + 0.0f};
    return box;
}
