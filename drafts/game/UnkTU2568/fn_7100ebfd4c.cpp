// Placeholder-named navigation-mesh query: for a position on a cell, finds the cell side it touches
// and records it in the result.
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct Vec2 {
    f32 x;
    f32 y;
    Vec2() {}
    Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    Vec2(const Vec2&);
    Vec2& operator=(const Vec2& o) {
        x = o.x;
        y = o.y;
        return *this;
    }
};

using Edge = UnkNavMeshEdge;
using Cell = UnkNavMeshCell;
using Mesh = UnkNavMesh;
using Segment = UnkSegment2D;

struct Result;
struct UnkSweep2D;
struct UnkContact2D;
struct UnkRegion_710027a41c;

// The result record as this function fills it (the placeholder Result of fn_7100ebfa7c.cpp).
struct UnkNavHit_bfd4c {
    u16 cell;
    u8 side;
    u8 pad_0x3;
    Vec2 point;  // 0x4
    u8 pad_0xc[0x20];
    u32 flags;  // 0x2c
};

Vec2* fn_710027a410(Mesh*, u16);
Vec2 fn_7100125a64(Vec2*);
bool fn_71002afcd8(Cell*, Edge*, u32);
bool fn_71002af6fc(UnkRegion_710027a41c*, u32);
bool fn_71002b007c(UnkNavMeshCell* cell);
bool fn_710012bcd0(const UnkSweep2D*, const UnkSegment2D*, UnkContact2D*);
bool fn_710027c7c4(Mesh* mesh, u16* cell, u8* side, u32 flags, f32 limit);

bool fn_7100ebfd4c(UnkNavMeshManager* self, Vec2* pos, u16 cell_id, Result* result, u32 flags, f32 a, f32 b) {
    UnkNavHit_bfd4c* out = reinterpret_cast<UnkNavHit_bfd4c*>(result);
    u16 cell_slot = cell_id;
    Mesh* mesh = self->navmesh;
    if (!mesh)
        return false;
    u32 found = 0;
    if (cell_id != 0xffff) {
        Cell* cell = fn_710027a328(mesh, cell_id);
        u8 side = 0;
        while (side < cell->field_0x10) {
            UnkNavMeshSidePair ids = fn_71002afe60(cell, &side);
            Vec2* v0 = fn_710027a410(mesh, ids.a);
            Vec2* v1 = fn_710027a410(mesh, ids.b);
            Segment seg(*reinterpret_cast<base::math::CVector2D*>(v0), *reinterpret_cast<base::math::CVector2D*>(v1));
            if (fn_710012bcd0(reinterpret_cast<const UnkSweep2D*>(pos), &seg,
                              reinterpret_cast<UnkContact2D*>(&out->point))) {
                Edge* edge = fn_71002afbe4(cell, &side);
                u16 target = cell_id;
                if (fn_71002afcd8(cell, edge, flags) &&
                    fn_71002af6fc(reinterpret_cast<UnkRegion_710027a41c*>(cell), flags)) {
                    f32 limit = a;
                    if (b >= 0.0f) {
                        if (fn_71002b005c(cell) || fn_71002b005c(fn_710027a328(mesh, edge->field_0x2)) ||
                            fn_71002b007c(cell) || fn_71002b007c(fn_710027a328(mesh, edge->field_0x2)))
                            limit = b;
                    }
                    if (fn_710027c7c4(self->navmesh, &cell_slot, &side, flags, limit)) {
                        found = fn_7100ebfd4c(self, pos, edge->field_0x2, result, flags, a, b);
                        break;
                    }
                    target = cell_slot;
                }
                u32 hit_flags = out->flags;
                out->cell = target;
                u8 hit_side = side;
                hit_flags |= 6;
                out->flags = hit_flags;
                out->side = hit_side;
                Vec2 d = fn_7100125a64(pos);
                f32 sx = d.x * 0.01f;
                f32 sy = d.y * 0.01f;
                f32 nx = out->point.x - sx;
                f32 ny = out->point.y - sy;
                out->flags |= 8;
                out->point.x = nx;
                out->point.y = ny;
                found = 1;
                break;
            }
            ++side;
        }
    }
    return found != 0;
}
