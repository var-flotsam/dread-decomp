// navmesh::CNavMesh (tools/tu_map.py unit 355, fn_7100274548 to fn_71002810b0): the navigation
// mesh the manager of game/UnkNavMeshManager.h holds. Its cells (game/UnkNavMeshCell.h) are
// 0x70-byte records at +0x290 (count at +0x298), and a flattened bounding-box tree of them sits at
// +0x400 (count at +0x408). This file holds the walk round a cell corner (fn_710027c460 and its
// recursive twin fn_710027c8c8, which carries the cells already passed), the searches for the
// cell side nearest a point (fn_710027b780 through the tree with the leaf visitor fn_71002807a0,
// fn_710027bbf0 by flooding the cells around the point with the ray probe fn_710027c134) and the
// collection of a cell's sides (fn_710027dab8). Most functions in this unit are member functions
// and lambdas in the original; here they take the mesh or the closure as their first argument.
#include <algorithm>
#include <new>

#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The navmesh manager unit's position and query result, as src/game_hi/fn_7100ebfa7c.cpp names
// them in its declaration of fn_7100ebfd4c (local types there); this file builds them with the
// layouts below.
struct Vec2;
struct Result;

// The query result fn_7100ebfd4c fills (Result's layout): the cell and side found, then points.
struct UnkNavMeshQueryResult {
    u16 cell;
    u8 side;
    CVector2D field_0x4;
    CVector2D field_0xc;
    CVector2D field_0x14;
    CVector2D field_0x1c;
    CVector2D field_0x24;
    f32 field_0x2c;
};
static_assert(sizeof(UnkNavMeshQueryResult) == 0x30, "UnkNavMeshQueryResult layout");

using Cell = UnkNavMeshCell;
using Edge = UnkNavMeshEdge;

// A cell record of the mesh's array (the header spells out only the members the lookups use).
struct UnkNavMeshCellSlot {
    Cell cell;
    u8 pad[0x70 - sizeof(Cell)];
};
static_assert(sizeof(UnkNavMeshCellSlot) == 0x70, "UnkNavMeshCellSlot layout");

struct UnkNavMeshTreeNode;

// The mesh, as far as this file reads it.
struct UnkNavMeshLayout {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlot* cells;  // +0x290
    u32 cell_count;             // +0x298
    u8 pad_0x29c[0x400 - 0x29c];
    UnkNavMeshTreeNode* nodes;  // +0x400, the bounding-box tree (src/game_lo/fn_710027a41c.cpp)
    u32 node_count;             // +0x408
};

// An edge record with the members the header leaves in its padding: the side of the neighbour
// cell the edge is on (+0x4) and the width of the opening (+0x8).
struct UnkNavMeshEdgeLayout {
    u8 index;
    u8 pad_0x1;
    u16 neighbour;       // +0x2
    u8 neighbour_side;   // +0x4
    u8 pad_0x5[3];
    f32 width;           // +0x8
};

// A list of up to N cell ids. Its copy constructor copies only the used entries; destroying it
// empties it.
template <u32 N>
struct UnkNavMeshIdList {
    u32 count;
    u16 ids[N];

    UnkNavMeshIdList() : count(0) {}
    UnkNavMeshIdList(const UnkNavMeshIdList& other) {
        u32 n = other.count;
        __builtin_memcpy(ids, other.ids, (u64)n * sizeof(u16));
        count = n;
    }
    ~UnkNavMeshIdList() { count = 0; }

    u16* begin() { return ids; }
    u16* end() { return ids + count; }
    bool Contains(u16 id) { return std::find(begin(), end(), id) != end(); }
    void Push(u16 id) {
        ids[count] = id;
        count = count + 1;
    }
};

// The cells a walk has passed through (passed by value).
using UnkNavMeshVisited = UnkNavMeshIdList<32>;
static_assert(sizeof(UnkNavMeshVisited) == 0x44, "UnkNavMeshVisited layout");

bool fn_71002afcd8(Cell*, Edge*, u32);
f32 fn_710027d34c(UnkNavMesh* self, const u16* cell, const u8* side, u32 flags, f32 width);
bool fn_710027c8c8(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side, UnkNavMeshVisited visited);

static inline UnkNavMeshLayout* L(UnkNavMesh* self) { return reinterpret_cast<UnkNavMeshLayout*>(self); }
static inline const UnkNavMeshEdgeLayout* L(const Edge* e) { return reinterpret_cast<const UnkNavMeshEdgeLayout*>(e); }

// The number of sides of a cell: the header's byte is the low byte of a word.
static inline u8 SideCount(const Cell* c) { return (u8)*reinterpret_cast<const u32*>(&c->field_0x10); }

// The next side of `cell` going round it forwards or backwards.
static inline __attribute__((always_inline)) u8 StepSide(UnkNavMesh* self, u16 cell, u8 side, bool forward) {
    if (forward)
        return SideCount(&L(self)->cells[cell].cell) - 1 == side ? 0 : side + 1;
    if (side == 0)
        side = SideCount(&L(self)->cells[cell].cell);
    return side - 1;
}

static inline __attribute__((always_inline)) bool IsZero(f32 v) { return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f; }

// Whether the edge on `side` of `cell` is crossable and wide enough for an object of half width
// `half`.
static inline __attribute__((always_inline)) bool Fits(UnkNavMesh* self, u32 index, u16 cell, const u16& key,
                                                       const u8& side, f32 half, u32 flags) {
    if (index < L(self)->cell_count && side < SideCount(&L(self)->cells[cell].cell)) {
        Cell* c = &L(self)->cells[cell].cell;
        Edge* e = fn_71002afbe4(c, &side);
        if (fn_71002afcd8(c, e, flags)) {
            if (IsZero(half))
                return true;
            f32 full = half * 2.0f;
            if (full <= L(e)->width)
                return true;
            if (full <= fn_710027d34c(self, &key, &side, flags, full))
                return true;
        }
    }
    return false;
}

// The walk round a corner: from side `side` of `cell` to the next side (forwards or backwards).
// A wall there ends the walk on it; an opening wide enough for widths->y is crossed, and the walk
// goes on from the neighbour cell (fn_710027c8c8) unless the neighbour's next side is a wall,
// where it ends; an opening only wide enough for widths->x ends it with no result. Returns whether
// it ended on a side (`out_cell`, `out_side`).
// NON_MATCHING(minor): the original masks fn_710027c8c8's result (`and w0, w0, #1`) in the call's
// own block where this masks the result in the shared epilogue, and it drops the dead store that
// empties the temporary list after that call; two cell address computations use other scratch
// registers.
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side) {
    u16 id = cell;
    u32 index = id;
    *out_cell = 0xffff;
    *out_side = 0xff;
    if (index != 0xffff && side != 0xff) {
        Cell* c = &L(self)->cells[id].cell;
        u8 next = StepSide(self, id, side, forward);
        const Edge* e = fn_71002afc34(c, &next);
        if (!fn_71002afcd8(c, const_cast<Edge*>(e), flags)) {
            *out_cell = id;
            *out_side = next;
        } else if (Fits(self, index, id, cell, next, widths->y * 0.5f, flags)) {
            u16 nb = L(e)->neighbour;
            Cell* n = &L(self)->cells[nb].cell;
            u8 nb_next = StepSide(self, nb, L(e)->neighbour_side, forward);
            if (fn_71002afcd8(n, const_cast<Edge*>(fn_71002afc34(n, &nb_next)), flags))
                return fn_710027c8c8(self, L(e)->neighbour, L(e)->neighbour_side, forward, widths, flags, out_cell,
                                     out_side, UnkNavMeshVisited());
            *out_cell = nb;
            *out_side = nb_next;
        } else if (Fits(self, index, id, cell, next, widths->x * 0.5f, flags)) {
            *out_cell = 0xffff;
            *out_side = 0xff;
        } else {
            *out_cell = id;
            *out_side = next;
        }
    }
    return (*out_cell != 0xffff) & (*out_side != 0xff);
}

// The same walk once it has crossed into another cell: fails when it comes back to a cell it has
// passed through.
// NON_MATCHING(minor): the original masks the recursive call's result and the early `false` in a
// block of their own where this masks the result in the shared epilogue; it keeps `flags` in w21
// where this spills it to the stack; the visited scan derives its end and its empty test from the
// byte count where this uses the count.
bool fn_710027c8c8(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side, UnkNavMeshVisited visited) {
    if (visited.Contains(cell))
        return false;
    visited.Push(cell);
    u16 id = cell;
    u32 index = id;
    *out_cell = 0xffff;
    *out_side = 0xff;
    if (index != 0xffff && side != 0xff) {
        Cell* c = &L(self)->cells[id].cell;
        u8 next = StepSide(self, id, side, forward);
        const Edge* e = fn_71002afc34(c, &next);
        if (!fn_71002afcd8(c, const_cast<Edge*>(e), flags)) {
            *out_cell = id;
            *out_side = next;
        } else if (Fits(self, index, id, cell, next, widths->y * 0.5f, flags)) {
            u16 nb = L(e)->neighbour;
            Cell* n = &L(self)->cells[nb].cell;
            u8 nb_next = StepSide(self, nb, L(e)->neighbour_side, forward);
            if (fn_71002afcd8(n, const_cast<Edge*>(fn_71002afc34(n, &nb_next)), flags))
                return fn_710027c8c8(self, L(e)->neighbour, L(e)->neighbour_side, forward, widths, flags, out_cell,
                                     out_side, visited);
            *out_cell = nb;
            *out_side = nb_next;
        } else if (Fits(self, index, id, cell, next, widths->x * 0.5f, flags)) {
            *out_cell = 0xffff;
            *out_side = 0xff;
        } else {
            *out_cell = id;
            *out_side = next;
        }
    }
    return (*out_cell != 0xffff) & (*out_side != 0xff);
}

// A segment the probe queries the manager with: the start, the end and the difference (taken
// from the start as given, so the constructor reads it again).
struct UnkNavMeshRay {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkNavMeshRay(const CVector2D& from, f32 to_x, f32 to_y) {
        start.x = from.x;
        start.y = from.y;
        end.x = to_x;
        end.y = to_y;
        f32 dx = end.x - from.x;
        f32 dy = end.y - from.y;
        delta.x = dx;
        delta.y = dy;
    }
};

// The best side found so far: the cell, the side and the closest point on it. Its constructor
// (fn_7100287598) and destructor (fn_710028760c, empty) are out of line; it is copied word by word.
struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)
void fn_7100287598(UnkNavMeshHit* self);
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

// A list of hits with room for a fixed number (not known here) after the count.
struct UnkNavMeshHits {
    u32 count;

    UnkNavMeshHit* items() { return reinterpret_cast<UnkNavMeshHit*>(&count + 1); }
    void Clear() {
        for (u64 i = 0; i < count; ++i)
            items()[i].~UnkNavMeshHit();
        count = 0;
    }
    // The hit at `i`, constructing the ones up to it first when the list is shorter.
    UnkNavMeshHit& At(u64 i) {
        if (i >= count) {
            for (u32 j = count; j < i + 1; ++j)
                new (&items()[j]) UnkNavMeshHit;
            count = i + 1;
        }
        return items()[i];
    }
    void Push(const UnkNavMeshHit& hit) {
        new (&items()[count]) UnkNavMeshHit(hit);
        ++count;
    }
};

struct UnkDirectionFilter;  // src/game/UnkDirectionFilter.cpp

// The closure of the per-side visitor fn_710027c298 (a lambda of fn_710027bbf0, capturing by
// reference): it measures a side of a cell against the best one so far.
struct UnkNavMeshVisitor {
    UnkNavMesh* mesh;
    f32* best_distance;
    UnkNavMeshHit* best;
    const CVector2D* point;
    u32* flags;
    const void* field_0x28;  // fn_710027bbf0's stack arguments (the callbacks)
    const void* field_0x30;
    const void* field_0x38;
    UnkDirectionFilter* direction;
    u64* filter_arg;
    u32* filter_arg2;
    u32* filter_arg3;
};

// The closure of the ray probe fn_710027c134 (the other lambda of fn_710027bbf0): everything is
// captured by reference.
struct UnkNavMeshRayProbe {
    u16* cell;
    u32* flags;
    const CVector2D* pos;
    f32* best;
    UnkNavMeshHit* hit;
    f32* hit_distance;
    UnkNavMeshVisitor* visitor;
};

struct UnkNavMeshBox {
    CVector2D min;
    CVector2D max;
};

bool fn_7100ebfd4c(UnkNavMeshManager*, Vec2*, u16, Result*, u32, f32, f32);
void fn_710027c298(UnkNavMeshVisitor* self, u16 cell, u8 side);

static inline UnkNavMeshManager* GetManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

// Casts a ray of length `dist` along (dx, dy) from the position: where the manager stops it (the
// visitor sees the cell and side it hit, and a recorded hit takes its distance as the best),
// otherwise its end, grows the box.
// NON_MATCHING(minor): the two argument registers are moved to x19 and x20 in the other order
// (`mov x19, x1` before `mov x20, x0` in the original).
void fn_710027c134(const UnkNavMeshRayProbe* self, UnkNavMeshBox* box, f32 dx, f32 dy, f32 dist) {
    const CVector2D& pos = *self->pos;
    UnkNavMeshRay ray(pos, dx * dist + pos.x, dy * dist + pos.y);
    UnkNavMeshQueryResult r;
    r.cell = 0xffff;
    r.side = 0xff;
    r.field_0x4 = {0, 0};
    r.field_0xc = {0, 0};
    r.field_0x14 = {0, 0};
    r.field_0x1c = {0, 0};
    r.field_0x24 = {0, 0};
    r.field_0x2c = 0;
    f32 x, y;
    if (fn_7100ebfd4c(GetManager(), reinterpret_cast<Vec2*>(&ray.start), *self->cell, reinterpret_cast<Result*>(&r),
                      *self->flags, 0.0f, -1.0f)) {
        fn_710027c298(self->visitor, r.cell, r.side);
        if (self->hit->IsValid())
            *self->best = *self->hit_distance;
        CVector2D p;
        fn_710007b430(&p, reinterpret_cast<const base::math::CVector3D*>(&r.field_0x4));
        x = p.x;
        y = p.y;
    } else {
        x = ray.end.x;
        y = ray.end.y;
    }
    f32 min_x = box->min.x < x ? box->min.x : x;
    f32 min_y = box->min.y < y ? box->min.y : y;
    box->min.x = min_x;
    box->min.y = min_y;
    f32 max_x = box->max.x > x ? box->max.x : x;
    f32 max_y = box->max.y > y ? box->max.y : y;
    box->max.x = max_x;
    box->max.y = max_y;
}

// Types other files define locally, forward-declared so that the declarations below are the same.
struct UnkRegion_710027a41c;      // a cell record (src/game_lo/fn_710027a41c.cpp)
struct UnkSegment2D_71001262e0;   // a segment's two points (src/game_lo/fn_71001262e0.cpp)

bool fn_71002af6fc(UnkRegion_710027a41c*, u32);
f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);
bool fn_71006f379c(const UnkDirectionFilter* self, const UnkSegment2D* seg);
bool fn_71002afc84(const Cell* cell, const u8* side, u32 flags);

// The segment of a cell side (fn_71002afea8 fills the two points; the word after them starts at -1).
struct UnkNavMeshSideSegment {
    CVector2D start;
    CVector2D end;
    f32 field_0x10;
};
void fn_71002afea8(const Cell* cell, const u8* side, UnkNavMeshSideSegment* out);

// A callback with its context: the function is called with the context first.
template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);

    explicit operator bool() const { return fn != nullptr; }
    R operator()(A... args) const { return fn(ctx, args...); }
};

// The state of a nearest-side search through the mesh's bounding-box tree: the point, the cell
// flags, two optional callbacks (a side filter and a segment verdict: 0 defers to the direction
// filter, 1 accepts) and the best side so far.
struct UnkNavMeshSideQuery {
    CVector2D point;
    u32 flags;
    u8 pad_0xc[4];
    u64 field_0x10;  // passed on to the side filter
    u32 field_0x18;
    u32 field_0x1c;
    UnkDirectionFilter* direction;
    UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32> filter;  // cell, side, ...
    UnkNavMeshCallback<s32, UnkNavMeshSideSegment*> verdict;
    UnkNavMesh* mesh;
    UnkNavMeshHit best;
    f32 best_distance;

    UnkNavMeshSideQuery(const CVector2D& point_, u32 flags_, u64 filter_arg, u32 filter_arg2, u32 filter_arg3,
                        UnkDirectionFilter* direction_, decltype(filter) filter_, decltype(verdict) verdict_,
                        UnkNavMesh* mesh_)
        : point{point_.x, point_.y}, flags(flags_), field_0x10(filter_arg), field_0x18(filter_arg2),
          field_0x1c(filter_arg3), direction(direction_), filter(filter_), verdict(verdict_), mesh(mesh_),
          best_distance(3.4028235e38f) {}
};
static_assert(__builtin_offsetof(UnkNavMeshSideQuery, best) == 0x50, "UnkNavMeshSideQuery layout");
static_assert(sizeof(UnkNavMeshSideQuery) == 0x60, "UnkNavMeshSideQuery layout");

// A node of the bounding-box tree (as in src/game_lo/fn_710027a41c.cpp): leaves have a negative
// link whose low 16 bits are the cell.
struct UnkNavMeshTreeNode {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    s32 link;
};

// The leaf visitor of the search: for an enabled cell, every side that passes the filter and is a
// boundary (fn_71002afc84 false) is measured, and the closest accepted one is kept. Always goes on.
// Inline (the original is an inline member, which keeps fn_710027b780's test of its result).
inline __attribute__((noinline)) bool fn_71002807a0(UnkNavMeshSideQuery* self, const UnkNavMeshTreeNode* node) {
    s32 link = node->link;
    if (link < 0) {
        u16 cell = link;
        if (fn_71002af6fc(reinterpret_cast<UnkRegion_710027a41c*>(&L(self->mesh)->cells[cell]), self->flags)) {
            Cell* c = &L(self->mesh)->cells[cell].cell;
            for (u8 side = 0; side < c->field_0x10; ++side) {
                if (self->filter &&
                    !self->filter(link, side, nullptr, nullptr, self->field_0x10, self->field_0x18, self->field_0x1c))
                    continue;
                if (fn_71002afc84(c, &side, self->flags))
                    continue;
                UnkNavMeshSideSegment seg;
                seg.start = {0, 0};
                seg.end = {0, 0};
                seg.field_0x10 = -1.0f;
                fn_71002afea8(c, &side, &seg);
                CVector2D closest = {0, 0};
                f32 d = fn_71001262e0(reinterpret_cast<const UnkSegment2D_71001262e0*>(&seg), &self->point, &closest,
                                      nullptr);
                if (!(d < self->best_distance))
                    continue;
                bool accept;
                if (self->verdict && self->verdict(&seg) != 0)
                    accept = self->verdict(&seg) == 1;
                else
                    accept = fn_71006f379c(self->direction, reinterpret_cast<const UnkSegment2D*>(&seg));
                if (accept) {
                    self->best_distance = d;
                    self->best.cell = link;
                    self->best.side = side;
                    self->best.point.x = closest.x;
                    self->best.point.y = closest.y;
                }
            }
        }
    }
    return true;
}

// The 2D box of a 3D one (fn_710007b430 projects each corner).
struct UnkNavMeshBox2D {
    CVector2D min;
    CVector2D max;
};

// The sides nearest to `point` (within 1000 units through the tree, or over every cell when the
// mesh has none) that pass the filters: the closest one, if any, is added to `out` (cleared
// first). Returns whether `out` holds a hit.
bool fn_710027b780(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>& filter,
                   const UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>& verdict) {
    out->Clear();
    if (L(mesh)->node_count == 0) {
        UnkNavMeshHit best;
        f32 best_distance = 3.4028235e38f;
        for (u16 i = 0; i < L(mesh)->cell_count; ++i) {
            if (!fn_71002af6fc(reinterpret_cast<UnkRegion_710027a41c*>(&L(mesh)->cells[i]), flags))
                continue;
            Cell* c = &L(mesh)->cells[i].cell;
            for (u8 side = 0; side < c->field_0x10; ++side) {
                if (filter && !filter(i, side, nullptr, nullptr, filter_arg, filter_arg2, filter_arg3))
                    continue;
                if (fn_71002afc84(c, &side, flags))
                    continue;
                UnkNavMeshSideSegment seg;
                seg.start = {0, 0};
                seg.end = {0, 0};
                seg.field_0x10 = -1.0f;
                fn_71002afea8(c, &side, &seg);
                CVector2D closest = {0, 0};
                f32 d = fn_71001262e0(reinterpret_cast<const UnkSegment2D_71001262e0*>(&seg), point, &closest,
                                      nullptr);
                if (!(d < best_distance))
                    continue;
                bool accept;
                if (verdict && verdict(&seg) != 0)
                    accept = verdict(&seg) == 1;
                else
                    accept = fn_71006f379c(direction, reinterpret_cast<const UnkSegment2D*>(&seg));
                if (accept) {
                    best.cell = i;
                    best.side = side;
                    best.point.x = closest.x;
                    best.point.y = closest.y;
                    best_distance = d;
                }
            }
        }
        if (best.IsValid())
            out->Push(best);
        return out->count != 0;
    }

    UnkNavMeshSideQuery query(*point, flags, filter_arg, filter_arg2, filter_arg3, direction, filter, verdict, mesh);
    UnkNavMeshBox2D box;
    f32 x = point->x;
    f32 y = point->y;
    CVector3D corners[2] = {{x - 1000.0f, y - 1000.0f, -3.4028235e38f}, {x + 1000.0f, y + 1000.0f, 3.4028235e38f}};
    fn_710007b430(&box.min, &corners[0]);
    fn_710007b430(&box.max, &corners[1]);
    u32 count = L(mesh)->node_count;
    for (u32 i = 0; i < count;) {
        UnkNavMeshTreeNode* node = &L(mesh)->nodes[i];
        if (!(box.max.x >= node->min_x && box.min.x <= node->max_x && box.max.y >= node->min_y &&
              box.min.y <= node->max_y)) {
            i = node->link >= 0 ? node->link : i + 1;
            continue;
        }
        if (node->link < 0 && !fn_71002807a0(&query, node))
            break;
        i++;
    }
    if (query.best.IsValid())
        out->Push(query.best);
    return out->count != 0;
}

struct UnkEntityObj;  // fn_71002b005c's parameter as src/game_aca57c/fn_7100ad0294.cpp declares it
struct UnkVec3;       // fn_710007b444's source as src/game_s2/fn_71003de7b4.cpp declares it
bool fn_71002b005c(UnkEntityObj*);
void fn_710007b444(void*, const UnkVec3*);

// A read-only zero 2D vector (the widths fn_710027dab8 walks corners with).
extern const CVector2D g_unk_7101648c8c __attribute__((visibility("hidden")));

// A hit on a side moves to the given position along the axis the side faces: x for a side whose
// normal is within about 25 degrees of the x axis, y otherwise.
static inline __attribute__((always_inline)) void AlignHit(UnkNavMeshHit* hit, const UnkNavMeshEdge* e,
                                                           const CVector2D* point) {
    f32 d = e->field_0xc * 1.0f + e->field_0x10 * 0.0f;
    if (d > 0.9f || d < -0.9f)
        hit->point.x = point->x;
    else
        hit->point.y = point->y;
}

// Adds to `out` the boundary sides of `cell` that it does not hold yet and that pass the filter
// (their midpoints, aligned with `point`), and for each opening the side the walk round the next
// corner ends on, or else the side a 100-unit ray along the opening's normal reaches. Fails only
// for no cell or one fn_71002b005c rejects.
// NON_MATCHING(minor): the original keeps the growth path of At() in the duplicate scan (never
// taken: the index is always below the count, so this build drops it) and spills the address of
// the side count; the scan's loop shape, register allocation and frame layout differ.
bool fn_710027dab8(UnkNavMesh* mesh, const CVector2D* point, u16 cell, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, const UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>& filter,
                   UnkNavMeshHits* out) {
    if (cell == 0xffff)
        return false;
    Cell* c = &L(mesh)->cells[cell].cell;
    if (!fn_71002b005c(reinterpret_cast<UnkEntityObj*>(c)))
        return false;
    for (u8 side = 0; side < c->field_0x10; ++side) {
        bool known = false;
        for (u64 i = 0; i < out->count; ++i) {
            if (out->items()[i].cell == cell && out->At(i).side == side) {
                known = true;
                break;
            }
        }
        if (known)
            continue;
        if (!fn_71002afc84(c, &side, flags)) {
            if (filter && !filter(cell, side, nullptr, nullptr, filter_arg, filter_arg2, filter_arg3))
                continue;
            UnkNavMeshSideSegment seg;
            seg.start = {0, 0};
            seg.end = {0, 0};
            seg.field_0x10 = -1.0f;
            fn_71002afea8(c, &side, &seg);
            const UnkNavMeshEdge* e = fn_71002afbe4(c, &side);
            UnkNavMeshHit hit;
            hit.cell = cell;
            hit.side = side;
            hit.point.x = (seg.start.x + seg.end.x) * 0.5f;
            hit.point.y = (seg.start.y + seg.end.y) * 0.5f;
            AlignHit(&hit, e, point);
            out->Push(hit);
            continue;
        }
        u8 next = SideCount(c) - 1 == side ? 0 : side + 1;
        if (!fn_71002afc84(c, &next, flags))
            continue;
        u16 corner_cell = 0xffff;
        u8 corner_side = 0xff;
        if (fn_710027c460(mesh, cell, side, true, &g_unk_7101648c8c, flags, &corner_cell, &corner_side)) {
            UnkNavMeshSideSegment seg;
            seg.start = {0, 0};
            seg.end = {0, 0};
            seg.field_0x10 = -1.0f;
            fn_71002afea8(c, &corner_side, &seg);
            const UnkNavMeshEdge* e = fn_71002afbe4(c, &corner_side);
            UnkNavMeshHit hit;
            hit.cell = corner_cell;
            hit.side = corner_side;
            hit.point.x = seg.start.x;
            hit.point.y = seg.start.y;
            AlignHit(&hit, e, point);
            out->Push(hit);
            continue;
        }
        UnkNavMeshSideSegment seg;
        seg.start = {0, 0};
        seg.end = {0, 0};
        seg.field_0x10 = -1.0f;
        fn_71002afea8(c, &side, &seg);
        const UnkNavMeshEdge* e = fn_71002afbe4(c, &side);
        UnkNavMeshRay ray(seg.start, e->field_0xc * 100.0f + seg.start.x, e->field_0x10 * 100.0f + seg.start.y);
        UnkNavMeshQueryResult r;
        r.cell = 0xffff;
        r.side = 0xff;
        r.field_0x4 = {0, 0};
        r.field_0xc = {0, 0};
        r.field_0x14 = {0, 0};
        r.field_0x1c = {0, 0};
        r.field_0x24 = {0, 0};
        r.field_0x2c = 0;
        if (fn_7100ebfd4c(GetManager(), reinterpret_cast<Vec2*>(&ray.start), cell, reinterpret_cast<Result*>(&r), flags,
                          0.0f, -1.0f)) {
            UnkNavMeshHit hit;
            hit.cell = r.cell;
            hit.side = r.side;
            fn_710007b444(&hit.point, reinterpret_cast<const UnkVec3*>(&r.field_0x4));
            out->Push(hit);
        }
    }
    return true;
}

void fn_710027a92c(UnkNavMesh* self, const UnkNavMeshBox* box, u32 flags, u32, u32, UnkNavMeshIdList<1024>* out);

// The side nearest to `point` that passes the filters, searched over the cells reachable from the
// one under the point without leaving the box the four probes (up, down, left and right, each at
// most its length and the best distance so far) span: the visitor measures every side of each
// cell reached. The hit, if any, is added to `out` (cleared first); returns whether `out` holds one.
// NON_MATCHING(minor): register allocation (the original keeps `out` in x19 without a spill, so
// its frame is 0x10 smaller) and the id-list scans compute their end from the byte count.
bool fn_710027bbf0(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out, const void* arg_0x28,
                   const void* arg_0x30, const void* arg_0x38, f32 up, f32 down, f32 left, f32 right) {
    out->Clear();
    UnkNavMeshHit best;
    f32 best_distance = 3.4028235e38f;
    UnkNavMeshVisitor visitor = {mesh,      &best_distance, &best,      point,       &flags,       arg_0x28,
                                 arg_0x30, arg_0x38,       direction, &filter_arg, &filter_arg2, &filter_arg3};
    u16 start = fn_710027a41c(mesh, point, 1);
    if (start != 0xffff) {
        UnkNavMeshIdList<1024> candidates;
        UnkNavMeshIdList<1024> pending;
        UnkNavMeshIdList<1024> done;
        UnkNavMeshBox box = {{point->x, point->y}, {point->x, point->y}};
        f32 reach = 3.4028235e38f;
        UnkNavMeshRayProbe probe = {&start, &flags, point, &reach, &best, &best_distance, &visitor};
        fn_710027c134(&probe, &box, 0.0f, -1.0f, std::min(reach, up));
        if (reach > 1.0f) {
            fn_710027c134(&probe, &box, 0.0f, 1.0f, std::min(reach, down));
            if (reach > 1.0f) {
                fn_710027c134(&probe, &box, -1.0f, 0.0f, std::min(reach, left));
                if (reach > 1.0f)
                    fn_710027c134(&probe, &box, 1.0f, 0.0f, std::min(reach, right));
            }
        }
        if (reach > 1.0f) {
            if (best.IsValid()) {
                box.min.x = point->x - reach;
                box.min.y = point->y - reach;
                box.max.x = reach + point->x;
                box.max.y = reach + point->y;
            }
            fn_710027a92c(mesh, &box, flags, 0, 0, &candidates);
            if (candidates.Contains(start) && !done.Contains(start) && !pending.Contains(start))
                pending.Push(start);
            while (pending.count != 0) {
                u16 cell = pending.ids[pending.count - 1];
                done.Push(cell);
                pending.count--;
                Cell* c = &L(mesh)->cells[cell].cell;
                u8 sides = SideCount(c);
                for (u8 side = 0; side < sides; ++side) {
                    fn_710027c298(&visitor, cell, side);
                    const UnkNavMeshEdgeLayout* e = L(fn_71002afbe4(c, &side));
                    u16 nb = e->neighbour;
                    if (nb != 0xffff && e->neighbour_side != 0xff && candidates.Contains(nb) && !done.Contains(nb) &&
                        !pending.Contains(nb))
                        pending.Push(nb);
                }
                std::sort(pending.begin(), pending.end());
            }
        }
    }
    if (best.IsValid())
        out->Push(best);
    return out->count != 0;
}

// ---- The mesh file loader (fn_710027563c) ----
//
// fn_710027563c reads a mesh from its file: the vertices, the cells (each read by fn_71002b0880,
// src/game/UnkNavMeshCellLoad.cpp) and, when there are both, the bounding-box tree (fn_710010ddc4);
// then five maps from string ids to per-cell data, a per-cell pointer table, the mesh items
// (src/game/navmesh/UnkNavMeshItem.cpp, each read by fn_71002b3524 and linked from the cells it
// covers), the links between items and last, for every side of every cell, the sides the walks
// round its two corners end on (fn_7100279fac). The containers are base::UnkVector arrays and maps
// of two parallel arrays (values, then string-id keys; src/game_s1/fn_71001358e0.cpp reserves most
// of them). Layouts and names are placeholders inferred from the code.
#include "UnkClone.h"
#include "base/UnkHashStream.h"
#include "base/UnkIdKindKey.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkObject_0x100.h"
#include "game_s2/UnkOwnedList.h"
#include "game_s2/UnkWeakHandle.h"

using base::global::CStrId;

// The file reader and the id and path pair, as src/game/CSubAreaManager.cpp names them.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710008d680(UnkStrPair_0x10* s, const char* path);
s64 fn_710007cee4(void* stream, const char** out);  // reads a string, returns it in *out
void fn_71000003d4(void*, const char*, bool);       // the string-id constructor (base/global/CStrId.h)

// A string id built out of line from a C string (fn_71000003d4), released on scope exit.
struct UnkNavLoadId {
    union {
        CStrId id;
    };
    explicit UnkNavLoadId(const char* str) { fn_71000003d4(&id, str, true); }
    ~UnkNavLoadId() { id.~CStrId(); }
};

__attribute__((always_inline)) static inline void Read(UnkFileReader* r, void* dest, u64 size) { fn_710007c714(&r->file, dest, size, 1); }
__attribute__((always_inline)) static inline const char* ReadName(UnkFileReader* r) {
    const char* s;
    fn_710007cee4(&r->file, &s);
    return s;
}

// A map from string ids to values: two arrays of `capacity` entries, grown together by an
// out-of-line reserve per value type (src/game_s1/fn_71001358e0.cpp's UnkIdMap).
template <class V>
struct UnkNavLoadMap {
    V* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    // The index of `key`, or the count when it is not there.
    u32 IndexOf(const CStrId& key) const {
        u32 i = 0;
        for (; i < count; ++i)
            if (keys[i].data == key.data)
                break;
        return i;
    }
    bool Contains(const CStrId& key) const { return IndexOf(key) != count; }
    // Copies the key in after the last one and returns the value slot beside it (the caller fills
    // it and counts it).
    V* AppendKey(const CStrId& key) {
        new (&keys[count]) CStrId(key);
        return &values[count];
    }
};
static_assert(sizeof(UnkNavLoadMap<u32>) == 0x28, "UnkNavLoadMap layout");

template <class V>
struct UnkIdMap;  // src/game_s1/fn_71001358e0.cpp
template <class V>
static inline UnkIdMap<UnkOwnedList>* AsIdMap(UnkNavLoadMap<V>* map) {
    return reinterpret_cast<UnkIdMap<UnkOwnedList>*>(map);
}
void fn_710027ee38(UnkIdMap<UnkOwnedList>* self, u32 capacity);
void fn_710027efe8(UnkIdMap<UnkOwnedList>* self, u32 capacity);
void fn_710027f198(UnkIdMap<UnkOwnedList>* self, u32 capacity);
void fn_710027f54c(UnkIdMap<UnkOwnedList>* self, u32 capacity);
void fn_710027f6fc(UnkIdMap<UnkOwnedList>* self, u32 capacity);
void fn_710027fe8c(UnkIdMap<UnkOwnedList>* self, u32 capacity);

// An empty map or array of the loader's kind (growth step 10, flags 0x57).
template <class V>
static inline void InitMap(UnkNavLoadMap<V>* map, base::UnkAllocator* allocator) {
    map->values = nullptr;
    map->keys = nullptr;
    map->count = 0;
    map->capacity = 0;
    map->step = 10;
    map->flags = 0x57;
    map->allocator = allocator;
}
template <class T>
static inline void InitVector(base::UnkVector<T>* v, T* data, u32 capacity, base::UnkAllocator* allocator) {
    v->data = data;
    v->count = 0;
    v->capacity = capacity;
    v->step = 10;
    v->field_0x14 = true;
    v->flags = 0x57;
    v->allocator = allocator;
}

// The capacity an append grows the array to: the next multiple of the step once it is full. A
// full array that cannot grow (no step) hangs, as in the original.
template <class T>
__attribute__((always_inline)) static inline u32 GrownCapacity(const base::UnkVector<T>* v) {
    u32 n = v->count;
    u32 grown = v->capacity;
    if (grown <= n && v->step != 0)
        grown = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)grown)
        for (;;) {
        }
    return grown;
}

// The element at `index` of an array whose reserve is out of line (`reserve`), growing a growable
// array to cover it first (`init` constructs the new elements).
template <class T, class Reserve, class Init>
__attribute__((always_inline)) static inline T* GrowAt(base::UnkVector<T>* v, u32 index, Reserve reserve, Init init) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0)
            cap = (index / v->step + 1) * v->step;
        reserve(v, cap);
        reserve(v, index + 1);
        for (u32 i = v->count; i < index + 1; ++i)
            init(&v->data[i]);
        v->count = index + 1;
    }
    return &v->data[index];
}

// The pointer arrays (one entry per cell) whose element accessor is a shared out-of-line copy
// (src/clones/).
struct UnkClone_fn_71002797bc;
struct UnkClone_fn_710027990c;
struct UnkClone_fn_7100279a5c;
using UnkNavLoadSlot3b0 = UnkCloneElem<UnkClone_fn_71002797bc, 8>;
using UnkNavLoadSlot338 = UnkCloneElem<UnkClone_fn_710027990c, 8>;
using UnkNavLoadSlot360 = UnkCloneElem<UnkClone_fn_7100279a5c, 8>;
extern template UnkNavLoadSlot3b0& base::UnkVector<UnkNavLoadSlot3b0>::GrowAt(u32);
extern template UnkNavLoadSlot338& base::UnkVector<UnkNavLoadSlot338>::GrowAt(u32);
extern template UnkNavLoadSlot360& base::UnkVector<UnkNavLoadSlot360>::GrowAt(u32);

template <class P, class S>
static inline P*& SlotAt(base::UnkVector<S>* v, u32 index) {
    return *reinterpret_cast<P**>(v->GrowAt(index).data);
}

// A vertex of the mesh, copied member by member.
struct UnkNavLoadVertex {
    f32 x;
    f32 y;

    UnkNavLoadVertex() : x(0), y(0) {}
    UnkNavLoadVertex(const UnkNavLoadVertex& other) : x(other.x), y(other.y) {}
};

struct UnkNavLoadItem;

// A cell record of the mesh's array (0x70 bytes; game/UnkNavMeshCell.h): its constructor
// (fn_71002af630) and destructor (fn_71002af6a0) are out of line, its copy is inline here (the edge
// array is copied by fn_710027ebe4).
struct UnkNavLoadCell {
    u16 id;
    base::UnkVector<u16> vertices;          // +0x8: the side count is the count's low byte
    base::UnkVector<UnkNavMeshEdge> edges;  // +0x30
    u64 field_0x58;                         // the mesh
    u64 field_0x60;
    UnkNavLoadItem* item;  // +0x68: the item the cell is in
};
static_assert(__builtin_offsetof(UnkNavLoadCell, edges) == 0x30, "UnkNavLoadCell layout");
static_assert(sizeof(UnkNavLoadCell) == 0x70, "UnkNavLoadCell layout");

// The members of an edge record (game/UnkNavMeshCell.h) the loader sets: the cells and edges the
// walks round the side's two corners end on.
struct UnkNavLoadEdgeLinks {
    u8 pad_0x0[0x18];
    void* field_0x18;
    UnkNavMeshEdge* field_0x20;
    void* field_0x28;
    UnkNavMeshEdge* field_0x30;
};
static_assert(sizeof(UnkNavLoadEdgeLinks) == sizeof(UnkNavMeshEdge), "UnkNavLoadEdgeLinks layout");

void fn_71002af630(UnkNavMeshCell* self);
void fn_71002af6a0(UnkNavMeshCell* self);
void fn_710027ebe4(base::UnkVector<UnkNavMeshEdge>* self, const base::UnkVector<UnkNavMeshEdge>* other);
// The cell array's reserve.
void fn_7100279600(base::UnkVector<UnkNavLoadCell>* self, u32 capacity);
// Reads cell `id` of `mesh` (src/game/UnkNavMeshCellLoad.cpp, with its own names for the types).
struct UnkNavCellData;
struct Mesh;
void fn_71002b0880(UnkNavCellData* cell, base::global::CRntFile* file, u16 id, Mesh* mesh);

static inline UnkNavMeshCell* AsCell(UnkNavLoadCell* c) { return reinterpret_cast<UnkNavMeshCell*>(c); }

// A cell built and destroyed by the out-of-line pair.
union UnkNavLoadCellBuf {
    UnkNavLoadCell cell;

    UnkNavLoadCellBuf() { fn_71002af630(AsCell(&cell)); }
    ~UnkNavLoadCellBuf() { fn_71002af6a0(AsCell(&cell)); }
};

// The cell copy: the vertex ids are copied into an array of the other's capacity.
__attribute__((always_inline)) static inline void CopyCell(UnkNavLoadCell* dst, const UnkNavLoadCell& src) {
    dst->id = src.id;
    dst->vertices.data = nullptr;
    dst->vertices.count = 0;
    dst->vertices.capacity = 0;
    dst->vertices.step = src.vertices.step;
    dst->vertices.field_0x14 = src.vertices.field_0x14;
    dst->vertices.flags = src.vertices.flags;
    dst->vertices.allocator = src.vertices.allocator;
    dst->vertices.Reserve(src.vertices.capacity);
    __builtin_memcpy(dst->vertices.data, src.vertices.data, (u64)src.vertices.count * sizeof(u16));
    dst->vertices.count = src.vertices.count;
    fn_710027ebe4(&dst->edges, &src.edges);
    dst->field_0x58 = src.field_0x58;
    dst->field_0x60 = src.field_0x60;
    dst->item = src.item;
}

struct UnkNavMeshItem;  // src/game/navmesh/UnkNavMeshItem.cpp

// A link of an item to another (0x40 bytes): the other item, its keys (base/UnkIdKindKey.h; the
// array is copied by fn_710027e258) and the side it is reached on (an UnkNavMeshHit at +0x30). Built
// from the other item by fn_71002b525c, read by fn_71002b5338 (src/base/UnkKeyListRead.cpp) and
// destroyed by fn_71002b52a4.
struct UnkNavLoadLink;
void fn_710027e258(void* self, const void* other);  // the key array's copy constructor
void fn_71002b525c(UnkNavLoadLink* self, UnkNavMeshItem* item);
void fn_71002b52a4(UnkNavLoadLink* self);
struct UnkKeyList;
void fn_71002b5338(UnkKeyList* self, base::global::CRntFile* file);

struct alignas(8) UnkNavLoadLink {
    u64 field_0x0;
    u8 keys[0x28];
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 pad_0x3c;

    UnkNavLoadLink(const UnkNavLoadLink& other) : field_0x0(other.field_0x0) {
        fn_710027e258(keys, other.keys);
        field_0x30 = other.field_0x30;
        field_0x34 = other.field_0x34;
        field_0x38 = other.field_0x38;
    }
    ~UnkNavLoadLink() { fn_71002b52a4(this); }
};
static_assert(sizeof(UnkNavLoadLink) == 0x40, "UnkNavLoadLink layout");

union UnkNavLoadLinkBuf {
    UnkNavLoadLink link;

    explicit UnkNavLoadLinkBuf(UnkNavMeshItem* item) { fn_71002b525c(&link, item); }
    ~UnkNavLoadLinkBuf() { link.~UnkNavLoadLink(); }
};

// The items (0x208 bytes), moved by fn_71002b3114 and destroyed by fn_71002b3328. The ids of the
// cells an item covers are at +0xc8, its links at +0xf0.
struct UnkNavMeshItemEntries;
struct UnkNavMeshItemPair;
void fn_71002b2f68(UnkNavMeshItem* self, u64 field_0x0, const CStrId& id, s32 field_0x10, bool flag,
                   const UnkNavMeshItemEntries* entries, const UnkNavMeshItemPair* pair, u16 field_0x1ec,
                   u8 field_0x1ee);
void fn_71002b3114(UnkNavMeshItem* self, UnkNavMeshItem* other);
void fn_71002b3328(UnkNavMeshItem* self);
void fn_71002b3524(UnkNavMeshItem* self, base::global::CRntFile* file);  // reads the item's data

struct alignas(8) UnkNavLoadItem {
    u8 raw[0x208];

    UnkNavLoadItem(UnkNavLoadItem&& other) { fn_71002b3114(Get(), other.Get()); }
    ~UnkNavLoadItem() { fn_71002b3328(Get()); }

    UnkNavMeshItem* Get() { return reinterpret_cast<UnkNavMeshItem*>(this); }
    base::UnkVector<u16>* Cells() { return reinterpret_cast<base::UnkVector<u16>*>(raw + 0xc8); }
    base::UnkVector<UnkNavLoadLink>* Links() { return reinterpret_cast<base::UnkVector<UnkNavLoadLink>*>(raw + 0xf0); }
};

union UnkNavLoadItemBuf {
    UnkNavLoadItem item;

    UnkNavLoadItemBuf() {}
    ~UnkNavLoadItemBuf() { item.~UnkNavLoadItem(); }
};

// The item array's element accessor (it grows a growable array to cover the index).
UnkNavMeshItem* fn_7100279dd4(base::UnkVector<UnkNavMeshItem>* self, u32 index);

// An entry of an item's list (UnkNavMeshItemEntry's layout): a key, a counted weak handle and a
// value.
struct UnkNavLoadEntry {
    CStrId key;
    UnkWeakHandle* handle;
    u32 value;

    UnkNavLoadEntry(const CStrId& key_, u32 value_) : key(key_), handle(nullptr), value(value_) {}
    UnkNavLoadEntry(const UnkNavLoadEntry& other) : key(other.key) {
        handle = nullptr;
        AssignRef(&handle, other.handle);
        value = other.value;
    }
    ~UnkNavLoadEntry() {
        if (UnkWeakHandle* h = handle) {
            if (--h->ref_count == 0)
                h->vf_0x10();
        }
    }
};
static_assert(sizeof(UnkNavLoadEntry) == 0x18, "UnkNavLoadEntry layout");

// A list of up to 8 entries stored right after the count (4-byte aligned, as fn_71002b2f68 reads
// it); destroying it empties it.
struct UnkNavLoadEntryList {
    u32 count;
    u32 storage[8 * sizeof(UnkNavLoadEntry) / 4];

    UnkNavLoadEntryList() : count(0) {}
    UnkNavLoadEntryList(const UnkNavLoadEntryList& other) {
        for (u32 i = 0; i < other.count; ++i)
            new (At(i)) UnkNavLoadEntry(*other.At(i));
        count = other.count;
    }
    ~UnkNavLoadEntryList() {
        for (u32 i = 0; i < count; ++i)
            At(i)->~UnkNavLoadEntry();
        count = 0;
    }

    UnkNavLoadEntry* At(u32 i) { return reinterpret_cast<UnkNavLoadEntry*>(&storage[i * (sizeof(UnkNavLoadEntry) / 4)]); }
    const UnkNavLoadEntry* At(u32 i) const {
        return reinterpret_cast<const UnkNavLoadEntry*>(&storage[i * (sizeof(UnkNavLoadEntry) / 4)]);
    }
};

// A 0x100-byte record (game/UnkObject_0x100.h; the members as src/game_lo/fn_71002b58c0.cpp
// copies them), built by fn_71002b57bc and destroyed by fn_71002b6094.
struct alignas(8) UnkNavLoadRecord {
    bool field_0x0;
    u8 field_0x1;
    u16 field_0x2;  // a cell
    u32 field_0x4;
    u32 field_0x8;
    CVector3D field_0xc;
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d;
    u16 field_0x1e;
    u32 field_0x20;
    u32 field_0x24;
    CVector3D field_0x28;
    u32 field_0x34;
    CStrId field_0x38;
    u8 field_0x40;
    u8 pad_0x41[3];
    u32 field_0x44;
    u32 field_0x48;
    u8 field_0x4c;
    u8 field_0x4d;
    u8 pad_0x4e[2];
    u32 field_0x50;
    u8 pad_0x54[4];
    base::UnkVector<u16> field_0x58;
    base::UnkVector<UnkIdKindKey> field_0x80;
    base::UnkVector<UnkIdKindKey> field_0xa8;
    base::UnkVector<UnkIdKindKey> field_0xd0;
    void* field_0xf8;
};
static_assert(__builtin_offsetof(UnkNavLoadRecord, field_0x38) == 0x38, "UnkNavLoadRecord layout");
static_assert(__builtin_offsetof(UnkNavLoadRecord, field_0x58) == 0x58, "UnkNavLoadRecord layout");
static_assert(sizeof(UnkNavLoadRecord) == 0x100, "UnkNavLoadRecord layout");

void fn_71002b57bc(UnkObject_0x100* self);
u32 fn_710026d2e4(const char* name);  // a name's enumerator

static inline UnkObject_0x100* AsObject(UnkNavLoadRecord* r) { return reinterpret_cast<UnkObject_0x100*>(r); }

union UnkNavLoadRecordBuf {
    UnkNavLoadRecord data;

    UnkNavLoadRecordBuf() { fn_71002b57bc(AsObject(&data)); }
    ~UnkNavLoadRecordBuf() { fn_71002b6094(AsObject(&data)); }
};

// A word and a record (src/game_s1/fn_71002a1778.cpp's UnkWordObject): the elements of the record
// arrays fn_7100279bac creates and fn_7100279ca0 appends to.
struct UnkNavLoadPathRecord {
    u32 field_0x0;
    union {
        UnkNavLoadRecord data;
    };

    explicit UnkNavLoadPathRecord(u32 word) {
        field_0x0 = word;
        fn_71002b57bc(AsObject(&data));
    }
    ~UnkNavLoadPathRecord() { fn_71002b6094(AsObject(&data)); }
};
static_assert(sizeof(UnkNavLoadPathRecord) == 0x108, "UnkNavLoadPathRecord layout");

struct UnkWordObject;
void fn_7100279bac(base::UnkVector<UnkWordObject>* self, u32 capacity, u32 flags);  // constructor
void fn_7100279ca0(base::UnkVector<UnkWordObject>* self, const UnkWordObject* value);
static inline base::UnkVector<UnkWordObject>* AsWordObjects(base::UnkVector<UnkNavLoadPathRecord>* v) {
    return reinterpret_cast<base::UnkVector<UnkWordObject>*>(v);
}

// The maps of +0x360 and +0x388 and their out-of-line parts.
using UnkNavLoadRecordMap = UnkNavLoadMap<base::UnkVector<UnkNavLoadPathRecord>>;
using UnkNavLoadRecordMaps = UnkNavLoadMap<UnkNavLoadRecordMap>;
using UnkNavLoadCellRecordMap = UnkNavLoadMap<base::UnkVector<UnkNavLoadSlot360>>;
void fn_710027f348(UnkNavLoadMap<UnkNavLoadCellRecordMap>* self, u32 capacity);
void fn_710027f8ac(UnkNavLoadRecordMaps* self);  // releases the keys and empties the values
void fn_710027f9d0(base::UnkVector<UnkNavLoadRecordMaps>* self, u32 capacity);
void fn_710027fb10(UnkNavLoadRecordMap* self);  // the same for the inner maps
void fn_710027fc88(UnkNavLoadRecordMaps* self, u32 capacity);

// A map taken over from `other`, which keeps only its capacity, growth and allocator.
template <class V>
static inline void MoveMap(UnkNavLoadMap<V>* dst, UnkNavLoadMap<V>* other) {
    *dst = *other;
    other->count = 0;
    other->values = nullptr;
    other->keys = nullptr;
}
// The destructor of those maps: emptied out of line (`clear`), then both arrays are freed.
template <class V, class Clear>
static inline void DestroyMap(UnkNavLoadMap<V>* map, Clear clear) {
    clear(map);
    if (map->keys)
        map->allocator->vf_0x48(map->keys);
    if (map->values)
        map->allocator->vf_0x48(map->values);
}

struct UnkNavMeshTree;  // the bounding-box tree at +0x400
void fn_710010ddc4(UnkNavMeshTree* self, base::global::CRntFile* file);
// The walk round a corner of a cell from one of its sides (forwards or backwards): the cell and
// side it ends on, or 0xffff and 0xff.
void fn_7100279fac(UnkNavMesh* self, const u16* cell, const u8* side, bool forward, bool flag, u16* out_cell,
                   u8* out_side);

// The members of the mesh the loader fills.
struct UnkNavLoadMesh {
    u8 pad_0x0[0x290];
    base::UnkVector<UnkNavLoadCell> cells;                          // +0x290
    base::UnkVector<UnkNavLoadVertex> vertices;                     // +0x2b8
    u8 pad_0x2e0[0x8];
    UnkNavLoadMap<base::UnkVector<u16>> field_0x2e8;                // cell id lists
    UnkNavLoadMap<base::UnkVector<UnkIdKindKey>> field_0x310;       // key lists
    UnkNavLoadMap<base::UnkVector<UnkNavLoadSlot338>> field_0x338;  // per cell: a record array
    UnkNavLoadMap<UnkNavLoadCellRecordMap> field_0x360;             // per name pair, per cell
    UnkNavLoadMap<base::UnkVector<UnkNavLoadRecordMaps>> field_0x388;
    base::UnkVector<UnkNavLoadSlot3b0> field_0x3b0;  // per cell
    base::UnkVector<UnkNavLoadItem> items;           // +0x3d8
    u8 tree[0x8];                                    // +0x400
};
static_assert(__builtin_offsetof(UnkNavLoadMesh, field_0x2e8) == 0x2e8, "UnkNavLoadMesh layout");
static_assert(__builtin_offsetof(UnkNavLoadMesh, field_0x3b0) == 0x3b0, "UnkNavLoadMesh layout");
static_assert(__builtin_offsetof(UnkNavLoadMesh, tree) == 0x400, "UnkNavLoadMesh layout");

// The cell at `index`, growing a growable array to cover it.
__attribute__((always_inline)) static inline UnkNavLoadCell* CellAt(UnkNavLoadMesh* m, u32 index) {
    return GrowAt(
        &m->cells, index, [](base::UnkVector<UnkNavLoadCell>* v, u32 n) { fn_7100279600(v, n); },
        [](UnkNavLoadCell* c) { fn_71002af630(AsCell(c)); });
}

__attribute__((always_inline)) static inline UnkNavLoadItem* ItemAt(UnkNavLoadMesh* m, u32 index) {
    return reinterpret_cast<UnkNavLoadItem*>(
        fn_7100279dd4(reinterpret_cast<base::UnkVector<UnkNavMeshItem>*>(&m->items), index));
}

// The members the three record lists share: three words narrowed to flags and counters, two
// pairs of words, two points, two names' enumerators and an id.
__attribute__((always_inline)) static inline void ReadRecordBody(UnkNavLoadRecord* r, UnkFileReader* f) {
    CVector3D point0 = {0, 0, 0};
    u32 pair0[2] = {0, 0};
    u32 pair1[2] = {0, 0};
    CVector3D point1 = {0, 0, 0};
    u32 w1;
    Read(f, &w1, 4);
    r->field_0x1 = w1;
    u32 w2;
    Read(f, &w2, 4);
    r->field_0x1e = w2;
    u32 w3;
    Read(f, &w3, 4);
    r->field_0x1c = w3;
    Read(f, pair0, 8);
    r->field_0x4 = pair0[0];
    r->field_0x8 = pair0[1];
    Read(f, pair1, 8);
    r->field_0x20 = pair1[0];
    r->field_0x24 = pair1[1];
    Read(f, &point0, 0xc);
    r->field_0xc = point0;
    Read(f, &point1, 0xc);
    r->field_0x28 = point1;
    r->field_0x18 = fn_710026d2e4(ReadName(f));
    r->field_0x34 = fn_710026d2e4(ReadName(f));
    UnkNavLoadId id(ReadName(f));
    r->field_0x38 = id.id;
}

// A key list: the count, then each key's id and kind as words.
__attribute__((always_inline)) static inline void ReadKeys(base::UnkVector<UnkIdKindKey>* keys, UnkFileReader* f) {
    s32 n = 0;
    Read(f, &n, 4);
    keys->Reserve(n);
    for (s32 i = 0; i < n; ++i) {
        u32 id;
        Read(f, &id, 4);
        u32 kind;
        Read(f, &kind, 4);
        UnkIdKindKey key(id, kind);
        keys->EmplaceBack(static_cast<UnkIdKindKey&&>(key));
    }
}

// The rest of a full record: a flag, two words, cell ids, three key lists, two flags and a word.
__attribute__((always_inline)) static inline void ReadRecordTail(UnkNavLoadRecord* r, UnkFileReader* f) {
    u8 b0 = 0;
    Read(f, &b0, 1);
    r->field_0x40 = b0;
    u32 w0 = 0;
    Read(f, &w0, 4);
    r->field_0x44 = w0;
    u32 w1 = 0;
    Read(f, &w1, 4);
    r->field_0x48 = w1;
    s32 n = 0;
    Read(f, &n, 4);
    r->field_0x58.Reserve(n);
    for (s32 i = 0; i < n; ++i) {
        u32 v;
        Read(f, &v, 4);
        u16 id = v;
        r->field_0x58.PushBack(id);
    }
    ReadKeys(&r->field_0x80, f);
    ReadKeys(&r->field_0xa8, f);
    ReadKeys(&r->field_0xd0, f);
    u8 b1 = 0;
    Read(f, &b1, 1);
    r->field_0x4c = b1;
    u8 b2 = 0;
    Read(f, &b2, 1);
    r->field_0x4d = b2;
    u32 w2 = 0;
    Read(f, &w2, 4);
    r->field_0x50 = w2;
}

// Reads the mesh from the file at `path` (see above for what it holds). The words the original
// reads without initialising them first (the narrowed flags and counters of the records, the cell
// of the last record kind, the key ids and kinds, the cell ids of the full records) are left
// uninitialised here too.
// NON_MATCHING(minor): the frame layout, register allocation and block order differ throughout
// (the same calls, loads and stores in the same order); the first key list's append
// (base::UnkVector<UnkIdKindKey>::EmplaceBack) is a weak out-of-line instantiation where the
// original inlines it; the graph array's element accessor keeps an element-construction loop the
// original drops (unreachable: it only runs once the count has wrapped round to 0); the "rb" mode
// literal is this file's own.
void fn_710027563c(UnkNavMesh* mesh, const char* path) {
    UnkNavLoadMesh* m = reinterpret_cast<UnkNavLoadMesh*>(mesh);
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d680(&s, path);
        base::UnkNamedEntry mode;
        fn_710007e704(&mode, "rb");
        opened = fn_710007c1ac(&reader, &s, reinterpret_cast<base::global::CRntString*>(&mode));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&mode));
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (opened) {
        UnkFileReader* f = &reader;
        fn_710007c00c(&reader.file, 8, 0);

        // The vertices.
        s32 nverts = 0;
        Read(f, &nverts, 4);
        if (nverts > 0) {
            m->vertices.Reserve(nverts);
            for (s32 i = 0; i < nverts; ++i) {
                UnkNavLoadVertex v;
                Read(f, &v, 8);
                m->vertices.PushBackCopy(v);
            }
        }

        // The cells, then the tree over them.
        s32 ncells = 0;
        Read(f, &ncells, 4);
        if (ncells > 0) {
            fn_7100279600(&m->cells, ncells);
            fn_7100279600(&m->cells, ncells);
            for (s32 i = 0; i < ncells; ++i) {
                {
                    UnkNavLoadCellBuf cell;
                    fn_7100279600(&m->cells, GrownCapacity(&m->cells));
                    CopyCell(&m->cells.data[m->cells.count], cell.cell);
                    ++m->cells.count;
                }
                fn_71002b0880(reinterpret_cast<UnkNavCellData*>(CellAt(m, m->cells.count - 1)), &reader.file, i,
                              reinterpret_cast<Mesh*>(mesh));
            }
        }
        if (nverts > 0 && ncells > 0)
            fn_710010ddc4(reinterpret_cast<UnkNavMeshTree*>(m->tree), &reader.file);

        // Named lists of cell ids (only the non-empty ones are added).
        s32 nlists = 0;
        Read(f, &nlists, 4);
        for (s32 i = 0; i < nlists; ++i) {
            const char* name = ReadName(f);
            s32 n = 0;
            Read(f, &n, 4);
            if (n > 0) {
                UnkNavLoadId key(name);
                base::UnkAllocator* a = fn_7100083990();
                auto& map = m->field_0x2e8;
                if (map.count == map.capacity)
                    fn_710027ee38(AsIdMap(&map), map.step + map.count);
                InitVector(map.AppendKey(key.id), static_cast<u16*>(nullptr), 0, a);
                ++map.count;
                base::UnkVector<u16>* list = &map.values[map.IndexOf(key.id)];
                list->Reserve(n);
                for (s32 j = 0; j < n; ++j) {
                    u32 v = 0;
                    Read(f, &v, 4);
                    u16 id = v;
                    list->PushBack(id);
                }
            }
        }

        // Named key lists, each made of groups of kinds for one id.
        s32 nkeys = 0;
        Read(f, &nkeys, 4);
        for (s32 i = 0; i < nkeys; ++i) {
            const char* name = ReadName(f);
            s32 n = 0;
            Read(f, &n, 4);
            UnkNavLoadId key(name);
            u32 capacity = n;
            base::UnkAllocator* a = fn_7100083990();
            UnkIdKindKey* data =
                capacity ? static_cast<UnkIdKindKey*>(a->vf_0x40((u64)capacity * sizeof(UnkIdKindKey), 8, 0x57))
                         : nullptr;
            auto& map = m->field_0x310;
            if (map.count == map.capacity)
                fn_710027efe8(AsIdMap(&map), map.step + map.count);
            InitVector(map.AppendKey(key.id), data, capacity, a);
            ++map.count;
            base::UnkVector<UnkIdKindKey>* keys = &map.values[map.IndexOf(key.id)];
            s32 ngroups = 0;
            Read(f, &ngroups, 4);
            for (s32 j = 0; j < ngroups; ++j) {
                u32 id = 0;
                Read(f, &id, 4);
                s32 nkinds = 0;
                Read(f, &nkinds, 4);
                for (s32 k = 0; k < nkinds; ++k) {
                    u32 kind = 0;
                    Read(f, &kind, 4);
                    UnkIdKindKey entry(id, kind);
                    keys->EmplaceBack(static_cast<UnkIdKindKey&&>(entry));
                }
            }
        }

        // A null pointer per cell.
        m->field_0x3b0.Reserve(ncells);
        for (s32 i = 0; i < ncells; ++i)
            SlotAt<void>(&m->field_0x3b0, i) = nullptr;

        // Named per-cell arrays of records.
        s32 nrecords = 0;
        Read(f, &nrecords, 4);
        for (s32 i = 0; i < nrecords; ++i) {
            UnkNavLoadId key(ReadName(f));
            base::UnkAllocator* a = fn_7100083990();
            auto& map = m->field_0x338;
            if (map.count == map.capacity)
                fn_710027f198(AsIdMap(&map), map.step + map.count);
            InitVector(map.AppendKey(key.id), static_cast<UnkNavLoadSlot338*>(nullptr), 0, a);
            ++map.count;
            base::UnkVector<UnkNavLoadSlot338>* cells = &map.values[map.IndexOf(key.id)];
            cells->Reserve(ncells);
            for (s32 j = 0; j < ncells; ++j)
                SlotAt<void>(cells, j) = nullptr;
            s32 nused = 0;
            Read(f, &nused, 4);
            for (s32 j = 0; j < nused; ++j) {
                u32 cell = 0;
                Read(f, &cell, 4);
                auto* list = new (fn_71000839a0(0x28, 0x57)) base::UnkVector<UnkObject_0x100>(0xf);
                SlotAt<base::UnkVector<UnkObject_0x100>>(cells, cell) = list;
                s32 n = 0;
                Read(f, &n, 4);
                for (s32 k = 0; k < n; ++k) {
                    UnkNavLoadRecordBuf rec;
                    rec.data.field_0x2 = cell;
                    ReadRecordBody(&rec.data, f);
                    SlotAt<base::UnkVector<UnkObject_0x100>>(cells, cell)->PushBackCopy(*AsObject(&rec.data));
                }
            }
        }

        // Records per cell under a pair of names.
        s32 nsets = 0;
        Read(f, &nsets, 4);
        for (s32 i = 0; i < nsets; ++i) {
            UnkNavLoadId key(ReadName(f));
            auto& outer = m->field_0x360;
            if (!outer.Contains(key.id)) {
                base::UnkAllocator* a = fn_7100083990();
                if (outer.count == outer.capacity)
                    fn_710027f348(&outer, outer.step + outer.count);
                InitMap(outer.AppendKey(key.id), a);
                ++outer.count;
            }
            UnkNavLoadCellRecordMap* inner = &outer.values[outer.IndexOf(key.id)];
            UnkNavLoadId key2(ReadName(f));
            base::UnkAllocator* a = fn_7100083990();
            if (inner->count == inner->capacity)
                fn_710027f54c(AsIdMap(inner), inner->step + inner->count);
            InitVector(inner->AppendKey(key2.id), static_cast<UnkNavLoadSlot360*>(nullptr), 0, a);
            ++inner->count;
            base::UnkVector<UnkNavLoadSlot360>* cells = &inner->values[inner->IndexOf(key2.id)];
            cells->Reserve(ncells);
            for (s32 j = 0; j < ncells; ++j)
                SlotAt<void>(cells, j) = nullptr;
            s32 nused = 0;
            Read(f, &nused, 4);
            for (s32 j = 0; j < nused; ++j) {
                u32 cell = 0;
                Read(f, &cell, 4);
                s32 n = 0;
                Read(f, &n, 4);
                auto* list = static_cast<base::UnkVector<UnkNavLoadPathRecord>*>(fn_71000839a0(0x28, 0x57));
                fn_7100279bac(AsWordObjects(list), n, 0x57);
                SlotAt<base::UnkVector<UnkNavLoadPathRecord>>(cells, cell) = list;
                for (s32 k = 0; k < n; ++k) {
                    s32 word = -1;
                    Read(f, &word, 4);
                    UnkNavLoadPathRecord rec(word);
                    rec.data.field_0x2 = cell;
                    ReadRecordBody(&rec.data, f);
                    ReadRecordTail(&rec.data, f);
                    fn_7100279ca0(AsWordObjects(SlotAt<base::UnkVector<UnkNavLoadPathRecord>>(cells, cell)),
                                  reinterpret_cast<const UnkWordObject*>(&rec));
                }
            }
        }

        // Named arrays of maps of maps of records.
        s32 ngraphs = 0;
        Read(f, &ngraphs, 4);
        for (s32 i = 0; i < ngraphs; ++i) {
            UnkNavLoadId key(ReadName(f));
            base::UnkAllocator* a = fn_7100083990();
            auto& map = m->field_0x388;
            if (map.count == map.capacity)
                fn_710027f6fc(AsIdMap(&map), map.step + map.count);
            InitVector(map.AppendKey(key.id), static_cast<UnkNavLoadRecordMaps*>(nullptr), 0, a);
            ++map.count;
            base::UnkVector<UnkNavLoadRecordMaps>* graphs = &map.values[map.IndexOf(key.id)];
            s32 ngraph = 0;
            Read(f, &ngraph, 4);
            for (s32 j = 0; j < ngraph; ++j) {
                {
                    UnkNavLoadRecordMaps tmp;
                    InitMap(&tmp, fn_7100083990());
                    fn_710027f9d0(graphs, GrownCapacity(graphs));
                    MoveMap(&graphs->data[graphs->count], &tmp);
                    ++graphs->count;
                    DestroyMap(&tmp, [](UnkNavLoadRecordMaps* p) { fn_710027f8ac(p); });
                }
                UnkNavLoadRecordMaps* graph = GrowAt(
                    graphs, graphs->count - 1,
                    [](base::UnkVector<UnkNavLoadRecordMaps>* v, u32 n) { fn_710027f9d0(v, n); },
                    [](UnkNavLoadRecordMaps* p) { InitMap(p, fn_7100083990()); });
                s32 nnodes = 0;
                Read(f, &nnodes, 4);
                for (s32 k = 0; k < nnodes; ++k) {
                    UnkNavLoadId key2(ReadName(f));
                    if (!graph->Contains(key2.id)) {
                        UnkNavLoadRecordMap tmp;
                        InitMap(&tmp, fn_7100083990());
                        if (graph->count == graph->capacity)
                            fn_710027fc88(graph, graph->step + graph->count);
                        MoveMap(graph->AppendKey(key2.id), &tmp);
                        ++graph->count;
                        DestroyMap(&tmp, [](UnkNavLoadRecordMap* p) { fn_710027fb10(p); });
                    }
                    UnkNavLoadRecordMap* node = &graph->values[graph->IndexOf(key2.id)];
                    const char* name3 = ReadName(f);
                    s32 n = 0;
                    Read(f, &n, 4);
                    UnkNavLoadId key3(name3);
                    u32 capacity = n;
                    base::UnkAllocator* a3 = fn_7100083990();
                    UnkNavLoadPathRecord* data =
                        capacity ? static_cast<UnkNavLoadPathRecord*>(
                                       a3->vf_0x40((u64)capacity * sizeof(UnkNavLoadPathRecord), 8, 0x57))
                                 : nullptr;
                    if (node->count == node->capacity)
                        fn_710027fe8c(AsIdMap(node), node->step + node->count);
                    InitVector(node->AppendKey(key3.id), data, capacity, a3);
                    ++node->count;
                    base::UnkVector<UnkNavLoadPathRecord>* list = &node->values[node->IndexOf(key3.id)];
                    for (s32 r = 0; r < n; ++r) {
                        s32 word = -1;
                        Read(f, &word, 4);
                        UnkNavLoadPathRecord rec(word);
                        u32 cell;
                        Read(f, &cell, 4);
                        rec.data.field_0x2 = cell;
                        ReadRecordBody(&rec.data, f);
                        ReadRecordTail(&rec.data, f);
                        fn_7100279ca0(AsWordObjects(list), reinterpret_cast<const UnkWordObject*>(&rec));
                    }
                }
            }
        }

        // The items, each linked from the cells it covers.
        s32 nitems = 0;
        Read(f, &nitems, 4);
        if (nitems > 0) {
            m->items.Reserve(nitems);
            for (s32 i = 0; i < nitems; ++i) {
                u8 flag = 0;
                s32 nentries = 0;
                const char* name = ReadName(f);
                Read(f, &flag, 1);
                Read(f, &nentries, 4);
                UnkNavLoadEntryList entries;
                for (s32 j = 0; j < nentries; ++j) {
                    u32 value = 0;
                    const char* entry = ReadName(f);
                    Read(f, &value, 4);
                    UnkNavLoadId id(entry);
                    new (entries.At(entries.count)) UnkNavLoadEntry(id.id, value);
                    ++entries.count;
                }
                u16 field_0x1ec = 0;
                Read(f, &field_0x1ec, 2);
                u8 field_0x1ee = 0xff;
                Read(f, &field_0x1ee, 1);
                u32 pair[2] = {0, 0};
                Read(f, &pair[0], 4);
                Read(f, &pair[1], 4);
                UnkNavLoadId id(name);
                {
                    // The list and the pair are passed by value.
                    UnkNavLoadEntryList entries_arg(entries);
                    u32 pair_arg[2] = {pair[0], pair[1]};
                    UnkNavLoadItemBuf item;
                    fn_71002b2f68(item.item.Get(), reinterpret_cast<u64>(mesh), id.id, i, flag != 0,
                                  reinterpret_cast<const UnkNavMeshItemEntries*>(&entries_arg),
                                  reinterpret_cast<const UnkNavMeshItemPair*>(pair_arg), field_0x1ec, field_0x1ee);
                    m->items.EmplaceBack(static_cast<UnkNavLoadItem&&>(item.item));
                }
                UnkNavLoadItem* it = ItemAt(m, m->items.count - 1);
                fn_71002b3524(it->Get(), &reader.file);
                base::UnkVector<u16>* covered = it->Cells();
                for (u16 *p = covered->data, *end = p + covered->count; p != end; ++p)
                    CellAt(m, *p)->item = it;
            }
        }

        // The links between items.
        s32 nlinks = 0;
        Read(f, &nlinks, 4);
        for (s32 i = 0; i < nlinks; ++i) {
            u32 from = 0xffffffff;
            Read(f, &from, 4);
            s32 n = 0;
            Read(f, &n, 4);
            if (n <= 0)
                continue;
            ItemAt(m, from)->Links()->Reserve(n);
            for (s32 j = 0; j < n; ++j) {
                u32 to = 0xffffffff;
                Read(f, &to, 4);
                UnkNavLoadItem* source = ItemAt(m, from);
                UnkNavLoadItem* target = ItemAt(m, to);
                {
                    UnkNavLoadLinkBuf link(target->Get());
                    source->Links()->PushBackCopy(link.link);
                }
                UnkNavLoadItem* a = ItemAt(m, from);
                UnkNavLoadItem* b = ItemAt(m, from);
                fn_71002b5338(reinterpret_cast<UnkKeyList*>(&a->Links()->data[b->Links()->count - 1]), &reader.file);
            }
        }

        // For each side of each cell, the sides the walks round its two corners end on.
        u32 count = m->cells.count;
        if (count != 0) {
            UnkNavLoadCell* end = m->cells.data + count;
            for (UnkNavLoadCell* c = m->cells.data; c != end; ++c) {
                for (u8 side = 0; side < (u8)c->vertices.count; ++side) {
                    auto* e = reinterpret_cast<UnkNavLoadEdgeLinks*>(
                        const_cast<UnkNavMeshEdge*>(fn_71002afc34(AsCell(c), &side)));
                    u16 next_cell = 0xffff;
                    u8 next_side = 0xff;
                    u16 id = c->id;
                    fn_7100279fac(mesh, &id, &side, true, true, &next_cell, &next_side);
                    if (next_cell != 0xffff && next_side != 0xff) {
                        UnkNavLoadCell* n = CellAt(m, next_cell);
                        e->field_0x18 = n;
                        e->field_0x20 = fn_71002afbe4(AsCell(n), &next_side);
                    }
                    u16 prev_cell = 0xffff;
                    u8 prev_side = 0xff;
                    u16 id2 = c->id;
                    fn_7100279fac(mesh, &id2, &side, false, true, &prev_cell, &prev_side);
                    if (prev_cell != 0xffff && prev_side != 0xff) {
                        UnkNavLoadCell* p = CellAt(m, prev_cell);
                        e->field_0x28 = p;
                        e->field_0x30 = fn_71002afbe4(AsCell(p), &prev_side);
                    }
                }
            }
        }
    }
    fn_710007c0e4(&reader.file);
}
