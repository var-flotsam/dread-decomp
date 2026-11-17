// CAbilityGhostAura unit: visits one side of a navigation-mesh cell while following a surface. Sides that
// turn down, turn too sharply or rise too far stop the walk; a long enough wall is probed for headroom and
// recorded as a landing point; other sides are recorded as they come. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU367.h"
#include "types.h"

using base::math::CVector2D;

// As src/game_hi/fn_7100ebfa7c.cpp defines it (its point and result types are local there).
struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

struct UnkNavPoint_l315a18 {
    f32 x;
    f32 y;
    UnkNavPoint_l315a18(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A segment with its direction (end - start).
struct UnkNavSegment_l315a18 {
    UnkNavPoint_l315a18 start;
    UnkNavPoint_l315a18 end;
    UnkNavPoint_l315a18 delta;
    UnkNavSegment_l315a18(const UnkNavPoint_l315a18& s, const UnkNavPoint_l315a18& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkNavResult_l315a18 {
    u16 cell;
    u8 side;
    UnkNavPoint_l315a18 points[5];
    f32 distance;
    UnkNavResult_l315a18()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// A recorded point of the walk, flagged when it is a landing.
struct UnkGhostAuraClimbPoint_l315a18 {
    bool landing;
    CVector2D pos;

    UnkGhostAuraClimbPoint_l315a18(bool landing, const CVector2D& pos) : landing(landing), pos(pos) {}
    UnkGhostAuraClimbPoint_l315a18(const UnkGhostAuraClimbPoint_l315a18& o) : landing(o.landing), pos(o.pos) {}
};

using UnkGhostAuraClimbPoints_l315a18 = base::global::CRntVector<UnkGhostAuraClimbPoint_l315a18>;

// The walk's state, each member captured by reference.
struct UnkGhostAuraClimb_l315a18 {
    const bool* forward;  // sides are walked from start to end
    bool* done;
    bool* blocked;    // whether the cell past the last side is blocked
    bool* on_wall;    // the last sides rose steeply
    bool* landed;
    f32* rise;        // how far the walk has risen along a wall
    const f32* extra_height;
    const u32* flags;  // passed to the probe
    bool* falling;     // a side turned down
    const CVector2D* direction;
    UnkGhostAuraClimbPoints_l315a18* points;
};

// CRntVector's append, growing to the next multiple of the step when full.
static inline void Append(UnkGhostAuraClimbPoints_l315a18* v, const UnkGhostAuraClimbPoint_l315a18& value) {
    u32 n = v->count;
    u32 cap = v->capacity;
    u32 new_capacity = cap;
    if (cap <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkGhostAuraClimbPoint_l315a18* p;
    if (cap < new_capacity) {
        p = static_cast<UnkGhostAuraClimbPoint_l315a18*>(
            v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkGhostAuraClimbPoint_l315a18), 8, v->flags));
        if (v->data) {
            UnkGhostAuraClimbPoint_l315a18* src = v->data;
            UnkGhostAuraClimbPoint_l315a18* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
                new (dst) UnkGhostAuraClimbPoint_l315a18(*src);
            v->allocator->vf_0x48(v->data);
        }
        n = v->count;
        v->data = p;
        v->capacity = new_capacity;
    } else {
        p = v->data;
    }
    new (&p[n]) UnkGhostAuraClimbPoint_l315a18(value);
    ++v->count;
}

static inline const CVector2D& From(const UnkNavMeshSideSegment& seg, bool forward) {
    return forward ? seg.start : seg.end;
}

static inline const CVector2D& To(const UnkNavMeshSideSegment& seg, bool forward) {
    return forward ? seg.end : seg.start;
}

static inline UnkNavPoint_l315a18 Sub(const CVector2D& a, const CVector2D& b) {
    return UnkNavPoint_l315a18(a.x - b.x, a.y - b.y);
}

static inline UnkNavPoint_l315a18 Add(const CVector2D& a, const CVector2D& b) {
    return UnkNavPoint_l315a18(a.x + b.x, a.y + b.y);
}

static inline f32 Length(f32 x, f32 y) {
    return sqrtf(x * x + y * y);
}

// Whether the cell on the far side of `edge` is blocked, into *blocked.
static inline void CheckNeighbour(const UnkNavMeshCell* cell, const UnkNavMeshEdge* edge, bool* blocked) {
    UnkNavMesh* mesh = reinterpret_cast<UnkNavMesh*>(cell->owner);
    if (mesh != nullptr && edge->field_0x2 != 0xffff)
        *blocked = fn_71002b007c(fn_710027a328(mesh, edge->field_0x2));
}

bool fn_7100315a18(const UnkGhostAuraClimb_l315a18* walk, const UnkNavMeshCell* cell, const UnkNavMeshEdge* edge) {
    UnkNavMeshSideSegment seg;
    seg.start.x = 0.0f;
    seg.start.y = 0.0f;
    seg.end.x = 0.0f;
    seg.end.y = 0.0f;
    seg.field_0x10 = -1.0f;
    if (!fn_71002afea8(cell, &edge->index, &seg))
        return false;

    f32 dx;
    f32 dy;
    if (*walk->forward) {
        UnkNavPoint_l315a18 d = Sub(seg.end, seg.start);
        dx = d.x;
        dy = d.y;
    } else {
        UnkNavPoint_l315a18 d = Sub(seg.start, seg.end);
        dx = d.x;
        dy = d.y;
    }
    f32 nx = dx;
    f32 ny = dy;
    f32 len = Length(dx, dy);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        nx = dx * inv;
        ny = dy * inv;
    }

    if (ny < 0.0f) {
        *walk->falling = true;
    } else if (acosf((nx * walk->direction->x + ny * walk->direction->y) /
                     (Length(nx, ny) * Length(walk->direction->x, walk->direction->y))) >= 0x1.919ca4p-1f) {
        *walk->done = true;
        CheckNeighbour(cell, edge, walk->blocked);
    } else if (fabsf(nx * walk->direction->x + ny * walk->direction->y) < 0.6f) {
        *walk->rise += fabsf(dy);
        if (*walk->rise > 110.0f) {
            *walk->done = true;
            *walk->on_wall = false;
            CheckNeighbour(cell, edge, walk->blocked);
        } else if (walk->points->count != 0) {
            *walk->on_wall = true;
        }
    } else if (*walk->on_wall) {
        // Off the wall: probe the headroom above the side's middle before landing on it.
        UnkNavPoint_l315a18 sum = *walk->forward ? Add(seg.start, seg.end) : Add(seg.end, seg.start);
        f32 height = (*walk->rise + *walk->rise) + (*walk->extra_height + *walk->extra_height);
        UnkNavPoint_l315a18 mid(sum.x * 0.5f, sum.y * 0.5f);
        UnkNavPoint_l315a18 up(0.0f, 1.0f);
        UnkNavPoint_l315a18 top(mid.x + height * up.x, mid.y + height * up.y);
        UnkNavSegment_l315a18 probe(mid, top);
        UnkNavResult_l315a18 result;
        if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&probe), reinterpret_cast<Result*>(&result), 0.0f,
                          *walk->flags, -1.0f))
            return false;
        *walk->rise = 0.0f;
        Append(walk->points, UnkGhostAuraClimbPoint_l315a18(*walk->on_wall, From(seg, *walk->forward)));
        *walk->landed = true;
        return false;
    }

    if (!*walk->done && !*walk->falling && !*walk->on_wall)
        Append(walk->points, UnkGhostAuraClimbPoint_l315a18(false, To(seg, *walk->forward)));
    return !*walk->done && !*walk->falling;
}
