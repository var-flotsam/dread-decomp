// The navmesh manager's unit (game/UnkNavMeshManager.h): fn_7100ebfd4c finds the side of a cell a
// position touches and records it in the result; fn_7100ebfa7c finds the nearest navigation
// point for a position on its current mesh (queries the start cell, then probes the cell's crossable
// edges that the position lies on and keeps the closer of the two results); fn_7100ebf278 is the
// manager's constructor.
#include "base/UnkParallelFind.h"
#include "base/UnkVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU110.h"
#include "game/UnkTU2568.h"
#include "game/UnkTU356.h"
#include "game/UnkTU367.h"
#include "game/UnkTU373.h"
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

using IdPair = UnkNavMeshSidePair;

using Segment = UnkSegment2D;

using Edge = UnkNavMeshEdge;
using Cell = UnkNavMeshCell;
using Mesh = UnkNavMesh;

struct Result {
    u16 field_0x0;
    u8 field_0x2;
    Vec2 field_0x4;
    Vec2 field_0xc;
    Vec2 field_0x14;
    Vec2 field_0x1c;
    Vec2 field_0x24;
    f32 field_0x2c;
    Result()
        : field_0x0(0xffff), field_0x2(0xff), field_0x4(0.0f, 0.0f), field_0xc(0.0f, 0.0f),
          field_0x14(0.0f, 0.0f), field_0x1c(0.0f, 0.0f), field_0x24(0.0f, 0.0f), field_0x2c(0.0f) {}
    Result& operator=(const Result& o) {
        field_0x0 = o.field_0x0;
        field_0x2 = o.field_0x2;
        field_0x4 = o.field_0x4;
        field_0xc = o.field_0xc;
        field_0x14 = o.field_0x14;
        field_0x1c = o.field_0x1c;
        field_0x24 = o.field_0x24;
        field_0x2c = o.field_0x2c;
        return *this;
    }
};



Vec2 fn_7100125a64(Vec2*);
bool fn_71002afcd8(Cell*, Edge*, u32);
Vec2* fn_710027a410(Mesh*, u16);

// Vec2 has CVector2D's layout (it only adds the out-of-line copy constructor); fn_710007b430 reads
// only the x and y of its source.
static inline base::math::CVector2D* AsV2(Vec2* v) { return reinterpret_cast<base::math::CVector2D*>(v); }
static inline const base::math::CVector3D* AsV3(Vec2* v) { return reinterpret_cast<const base::math::CVector3D*>(v); }

static inline f32 DistSq(const Vec2& a, const Vec2& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b) {
    if (self->navmesh == nullptr)
        return false;
    Vec2 p;
    p.x = pos->x;
    p.y = pos->y;
    s32 ret = 0;
    u16 cellId = fn_710027a41c(self->navmesh, AsV2(&p), flags);
    if (cellId != 0xffff) {
        ret = fn_7100ebfd4c(self, pos, cellId, out, flags, a, b);
        Mesh* mesh = self->navmesh;
        Cell* cell = fn_710027a328(mesh, cellId);
        Vec2 dir = fn_7100125a64(pos);
        for (u8 i = 0; i < cell->field_0x10; ++i) {
            Edge* edge = fn_71002afbe4(cell, &i);
            if (!fn_71002afcd8(cell, edge, flags))
                continue;
            f32 d = __builtin_fabsf(dir.x * edge->field_0xc + dir.y * edge->field_0x10);
            if (!(d >= 1.0f))
                continue;
            IdPair ids = fn_71002afe60(cell, &i);
            Vec2* v0 = fn_710027a410(mesh, ids.a);
            Vec2* v1 = fn_710027a410(mesh, ids.b);
            Segment seg(*AsV2(v0), *AsV2(v1));
            // Vec2 has CVector2D's layout (it only adds the out-of-line copy constructor).
            if (!fn_7100125c64(&seg, reinterpret_cast<const base::math::CVector2D*>(&p), false, 0.0f))
                continue;
            Result r;
            if (fn_7100ebfd4c(self, pos, edge->field_0x2, &r, flags, a, b)) {
                bool closer = true;
                if (ret) {
                    Vec2 t0;
                    fn_710007b430(AsV2(&t0), AsV3(&r.field_0x4));
                    f32 d0 = DistSq(p, t0);
                    Vec2 t1;
                    fn_710007b430(AsV2(&t1), AsV3(&out->field_0x4));
                    closer = d0 < DistSq(p, t1);
                }
                if (closer)
                    *out = r;
                ret = true;
            }
            break;
        }
    }
    return ret;
}

void fn_7100ebf278(UnkNavMeshManager* self) {
    self->navmesh = nullptr;
    fn_71002810b0(&self->state);
    fn_71002b4934(&self->reach);
    self->field_0x62e38 = nullptr;
    self->field_0x62e40 = nullptr;
    self->field_0x62e30 = nullptr;
    self->field_0x62e48 = 10;
    self->field_0x62e4c = 0x57;
    self->allocator = fn_7100083990();
    fn_7100ebf2fc(&self->pool, 100, 0, 0x57, nullptr);
}

// The smart-link group registry at +0x62e30 as parallel arrays: 16-byte entries (two pointers) and
// the names that key them.
struct UnkNavGroupEntry {
    void* first;
    void* second;
};
struct UnkNavGroupRegistry {
    u8 pad_0x0[0x62e30];
    UnkNavGroupEntry* values;
    base::global::CStrId* ids;
    u32 count;
};
static_assert(__builtin_offsetof(UnkNavGroupRegistry, values) == 0x62e30, "UnkNavGroupRegistry layout");

void* fn_7100ec1344(UnkNavMeshManager* self, const base::global::CStrId* name) {
    UnkNavGroupRegistry* r = reinterpret_cast<UnkNavGroupRegistry*>(self);
    UnkNavGroupEntry* slot = UnkParallelFindSlot(r, *name);
    return slot != &r->values[r->count] ? slot->first : nullptr;
}

void* fn_7100ec13cc(UnkNavMeshManager* self, const base::global::CStrId* name) {
    UnkNavGroupRegistry* r = reinterpret_cast<UnkNavGroupRegistry*>(self);
    UnkNavGroupEntry* slot = UnkParallelFindSlot(r, *name);
    return slot != &r->values[r->count] ? slot->second : nullptr;
}

// The result record as this function fills it (the placeholder Result of fn_7100ebfa7c.cpp).
struct UnkNavHit_bfd4c {
    u16 cell;
    u8 side;
    u8 pad_0x3;
    Vec2 point;  // 0x4
    u8 pad_0xc[0x20];
    u32 flags;  // 0x2c
};

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
                out->cell = target;
                u32 hit_flags = out->flags;
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
