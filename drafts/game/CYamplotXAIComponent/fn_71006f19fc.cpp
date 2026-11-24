// CYamplotXAIComponent's unit: walks the navigation mesh from a point along the cell sides, one way
// or the other (`mode`), spending `*budget` on the distance to each side's corner; true once the
// budget runs out, false when no cell holds the point or the next side turns away from the
// movement's direction. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::math::CVector2D;

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

struct UnkEnemyMovement_A5;
UnkVec2f_A5 fn_71010c9a1c(UnkEnemyMovement_A5* self);

struct UnkSource_7100286798;
u32 fn_7100413fd0(UnkSource_7100286798* self);

struct Mesh;
struct Vec2;
Vec2* CNavMesh_GetVertex(Mesh* mesh, u16 index);

bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

// From src/game/navmesh/CNavMesh.cpp (the same definition).
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

struct UnkDirectionFilter;
struct UnkNavMeshSideSegment;

template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);

    explicit operator bool() const { return fn != nullptr; }
    R operator()(A... args) const { return fn(ctx, args...); }
};

using UnkNavSideFilter = UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>;
using UnkNavSideVerdict = UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>;

bool fn_710027b780(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavSideFilter& filter, const UnkNavSideVerdict& verdict);

// A hit list with room for 16 hits, destroyed with its hits.
struct UnkNavHitList_l6f19fc : UnkNavMeshHits {
    alignas(4) u8 storage[16 * sizeof(UnkNavMeshHit)];

    UnkNavHitList_l6f19fc() { count = 0; }
    ~UnkNavHitList_l6f19fc() { Clear(); }
};

// The direction filter fn_710027b780 takes: two directions and a word.
struct UnkNavDirections_l6f19fc {
    CVector2D first;
    CVector2D second;
    u32 field_0x10;
    u8 pad_0x14[4];

    UnkNavDirections_l6f19fc(f32 x0, f32 y0, f32 x1, f32 y1) {
        first.x = x0;
        first.y = y0;
        second.x = x1;
        second.y = y1;
        field_0x10 = 0;
    }
};

struct UnkYamplotOwner_l6f19fc {
    u8 pad_0x0[0x218];
    UnkEnemyMovement_A5* movement;
};

struct UnkYamplot_l6f19fc {
    u8 pad_0x0[0x30];
    UnkYamplotOwner_l6f19fc* owner;
    u8 pad_0x38[0x14728 - 0x38];
    UnkSource_7100286798* field_0x14728;
};

static inline UnkNavMeshManager* NavManager_l6f19fc() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

// The side `side` of cell `cell` as a segment between its two corners.
static inline UnkSegment2D SideSegment_l6f19fc(UnkNavMeshManager* manager, u16 cell, u8* side) {
    UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a33c(manager->navmesh, cell), side);
    const CVector2D* a =
        reinterpret_cast<const CVector2D*>(CNavMesh_GetVertex(reinterpret_cast<Mesh*>(manager->navmesh), ends.a));
    const CVector2D* b =
        reinterpret_cast<const CVector2D*>(CNavMesh_GetVertex(reinterpret_cast<Mesh*>(manager->navmesh), ends.b));
    return UnkSegment2D(*a, *b);
}

// The cell and side past `side` of `cell`, going forward or back.
static inline bool NextSide_l6f19fc(UnkNavMeshManager* manager, u16 cell, u8 side, bool forward, u32 flags,
                                    u16* out_cell, u8* out_side) {
    CVector2D widths{100.0f, 100.0f};
    return fn_710027c460(manager->navmesh, cell, side, forward, &widths, flags, out_cell, out_side);
}

// The hits of the cell holding `point`, unfiltered.
static inline bool FindHits_l6f19fc(UnkNavMesh* mesh, const CVector2D* point, UnkNavMeshHits* hits,
                                    const UnkNavSideFilter& filter, const UnkNavSideVerdict& verdict) {
    UnkNavDirections_l6f19fc directions(0.0f, -1.0f, 0.0f, 1.0f);
    return fn_710027b780(mesh, point, 0, 0, 0, 0, reinterpret_cast<UnkDirectionFilter*>(&directions), hits, filter,
                         verdict);
}

bool fn_71006f19fc(UnkYamplot_l6f19fc* self, u32 mode, const CVector2D* point, f32* budget) {
    UnkNavMeshManager* manager = NavManager_l6f19fc();
    CVector2D pos{point->x, point->y};
    UnkVec2f_A5 dir = fn_71010c9a1c(self->owner->movement);
    UnkNavMesh* mesh = NavManager_l6f19fc()->navmesh;
    u16 cell = CNavMesh_FindCellIndexAtPoint(mesh, &pos, 0);
    UnkNavHitList_l6f19fc hits;
    UnkNavSideFilter no_filter{nullptr, nullptr};
    UnkNavSideVerdict no_verdict{nullptr, nullptr};
    if (!FindHits_l6f19fc(mesh, &pos, &hits, no_filter, no_verdict))
        return false;
    u8 side = hits.At(0).side;
    CVector2D at = pos;
    while (*budget > 0.0f) {
        UnkSegment2D edge = SideSegment_l6f19fc(manager, cell, &side);
        const CVector2D& corner = mode == 0 ? edge.start : edge.end;
        f32 dx = corner.x - at.x;
        f32 dy = corner.y - at.y;
        f32 dist = sqrtf(dx * dx + dy * dy);
        if (dist >= *budget)
            return true;
        *budget -= dist;

        u32 flags = fn_7100413fd0(self->field_0x14728);
        u16 next_cell;
        u8 next_side;
        if (!NextSide_l6f19fc(manager, cell, side, mode == 1, flags, &next_cell, &next_side))
            continue;
        side = next_side;
        cell = next_cell;
        UnkSegment2D next = SideSegment_l6f19fc(manager, cell, &side);
        at = mode == 1 ? next.start : next.end;
        UnkVec2Result normal = fn_7100125e78(&next);
        if (!(normal.x * dir.x + normal.y * dir.y > 0.99f))
            return false;
    }
    return true;
}
