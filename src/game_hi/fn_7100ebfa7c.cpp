// The navmesh manager's unit (game/UnkNavMeshManager.h): fn_7100ebfa7c finds the nearest navigation
// point for a position on its current mesh (queries the start cell, then probes the cell's crossable
// edges that the position lies on and keeps the closer of the two results); fn_7100ebf278 is the
// manager's constructor.
#include "base/UnkVector.h"
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

struct IdPair {
    u16 a;
    u16 b;
    IdPair(const IdPair&);
};

struct Segment {
    Vec2 p0;
    Vec2 p1;
    f32 field_0x10;
    f32 field_0x14;
};

struct Edge {
    u16 field_0x0;
    u16 field_0x2;
    u8 pad_0x4[0x8];
    f32 field_0xc;
    f32 field_0x10;
};

struct Cell {
    u8 pad_0x0[0x10];
    u8 field_0x10;
};

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



u16 fn_710027a41c(Mesh*, Vec2*, u32);
bool fn_7100ebfd4c(UnkNavMeshManager*, Vec2*, u16, Result*, u32, f32, f32);
Cell* fn_710027a328(Mesh*, u16);
Vec2 fn_7100125a64(Vec2*);
Edge* fn_71002afbe4(Cell*, u8*);
bool fn_71002afcd8(Cell*, Edge*, u32);
IdPair fn_71002afe60(Cell*, u8*);
Vec2* fn_710027a410(Mesh*, u16);
void fn_7100125b1c(Segment*, Vec2*, Vec2*);
bool fn_7100125c64(Segment*, Vec2*, bool, f32);
void fn_710007b430(Vec2*, Vec2*);

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
    u16 cellId = fn_710027a41c(self->navmesh, &p, flags);
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
            Segment seg;
            fn_7100125b1c(&seg, v0, v1);
            if (!fn_7100125c64(&seg, &p, false, 0.0f))
                continue;
            Result r;
            if (fn_7100ebfd4c(self, pos, edge->field_0x2, &r, flags, a, b)) {
                bool closer = true;
                if (ret) {
                    Vec2 t0;
                    fn_710007b430(&t0, &r.field_0x4);
                    f32 d0 = DistSq(p, t0);
                    Vec2 t1;
                    fn_710007b430(&t1, &out->field_0x4);
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

// The manager's constructor: no mesh yet, the query state and the reachability tester
// constructed in place, an empty smart-link group registry (growth step 10, allocation flags 0x57,
// the default allocator) and a pool of 100 registry nodes.
void fn_71002810b0(UnkNavMeshState_0x5ddf0* self);
void fn_71002b4934(UnkNavMeshReach_0x5038* self);
void fn_7100ebf2fc(UnkNavMeshNodePool_0x70* self, u32 count, u32 unused, u32 flags, base::UnkAllocator* allocator);

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
