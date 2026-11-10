// Unit 367 (data/tu_map.csv, no reliable name; game/UnkNavMeshCell.h): a navmesh cell's bounding box over its
// vertices, empty (maximum and minimum floats) for a cell without vertices or mesh. Names and layouts are
// placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkNavMesh;
struct Vec2;
using Mesh = UnkNavMesh;
Vec2* fn_710027a410(Mesh* mesh, u16 index);

struct UnkNavMeshBounds_b0252 {
    f32 min_x, min_y, max_x, max_y;
    UnkNavMeshBounds_b0252() {}
    UnkNavMeshBounds_b0252(const UnkNavMeshBounds_b0252& o)
        : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y) {}
};

struct UnkNavMeshCellBounds_b0252 {
    u8 pad_0x0[0x8];
    u16* vertices;  // 0x8
    u32 count;      // 0x10
    u8 pad_0x14[0x44];
    Mesh* mesh;  // 0x58
};

static inline const base::math::CVector2D* Vertex(UnkNavMeshCellBounds_b0252* c, u32 i) {
    return reinterpret_cast<const base::math::CVector2D*>(fn_710027a410(c->mesh, c->vertices[i]));
}

UnkNavMeshBounds_b0252 fn_71002aff5c(UnkNavMeshCellBounds_b0252* self) {
    UnkNavMeshBounds_b0252 b;
    if (self->mesh && self->count) {
        const base::math::CVector2D* first = Vertex(self, 0);
        const base::math::CVector2D* last = Vertex(self, 0);
        b.min_x = first->x;
        b.min_y = first->y;
        b.max_x = last->x;
        b.max_y = last->y;
        for (u64 i = 1; i < self->count; ++i) {
            const base::math::CVector2D* v = Vertex(self, i);
            b.min_x = b.min_x < v->x ? b.min_x : v->x;
            b.min_y = b.min_y < v->y ? b.min_y : v->y;
            b.max_x = b.max_x > v->x ? b.max_x : v->x;
            b.max_y = b.max_y > v->y ? b.max_y : v->y;
        }
    } else {
        const u64 lo = 0x7f7fffff7f7fffffull;
        const u64 hi = 0xff7fffffff7fffffull;
        __builtin_memcpy(&b.min_x, &lo, 8);
        __builtin_memcpy(&b.max_x, &hi, 8);
    }
    return b;
}
