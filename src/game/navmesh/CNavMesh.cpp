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
