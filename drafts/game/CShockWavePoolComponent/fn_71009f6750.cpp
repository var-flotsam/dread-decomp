// CShockWavePoolComponent's unit: where the pool's wave reaches along the navigation mesh. From the cell
// under `point` it walks half the pool's reach each way along the floor's sides, past corners that keep
// going in `dir`, and records the point where each end stops. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU362.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;

template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);
};

struct UnkDirectionFilter;
struct UnkNavMeshHits {
    u32 count;
};

using UnkNavSideFilter = UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>;
using UnkNavSideVerdict = UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>;

bool fn_710027b780(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavSideFilter& filter, const UnkNavSideVerdict& verdict);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);
struct Vec2;
Vec2* CNavMesh_GetVertex(UnkNavMesh* mesh, u16 index);
bool fn_71002afc84(UnkNavMeshCell* cell, const u8* index, u32 flags);

// The direction the found sides must face: none here (any angle, cosine -1).
struct UnkShockDirFilter_l9f6750 {
    u32 mode;
    f32 min_cos;
    CVector2D dir;
    f32 field_0x10;
};

struct UnkShockHit_l9f6750 {
    u16 cell;
    u8 side;
    u8 pad_0x3[0x9];
};

// The cells the query found, at most 17, destroyed in place.
struct UnkShockHits_l9f6750 {
    u32 count;
    UnkShockHit_l9f6750 items[17];

    UnkShockHits_l9f6750() : count(0) {}
    ~UnkShockHits_l9f6750() {
        for (u64 i = 0; i < count; ++i)
            fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(&items[i]));
    }
};

struct UnkShockWavePool_l9f6750 {
    u8 pad_0x0[0x15c];
    f32 reach;   // 0x15c
    f32 offset;  // 0x160
    u8 pad_0x164[0xe8];
    bool floor_only;  // 0x24c
    u8 pad_0x24d[0x17];
    u32 cross_flags;  // 0x264
    u8 pad_0x268[0x90];
    UnkVec3f back_end;  // 0x2f8
    UnkVec3f front_end;  // 0x304
    bool back_found;     // 0x310
    bool front_found;    // 0x311
};
static_assert(__builtin_offsetof(UnkShockWavePool_l9f6750, front_found) == 0x311, "layout");

static inline UnkNavMeshManager* NavManager_l9f6750() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

inline f32 Distance_l9f6750(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

inline f32 Dot_l9f6750(const UnkVec2Result& a, const CVector2D& b) {
    return a.x * b.x + a.y * b.y;
}

void fn_71009f6750(UnkShockWavePool_l9f6750* self, const CVector2D* point, const CVector2D* start,
                   const CVector2D* dir, bool* out) {
    UnkNavMesh* mesh = NavManager_l9f6750()->navmesh;
    u16 cell = CNavMesh_FindCellIndexAtPoint(mesh, point, 0);
    UnkShockHits_l9f6750 hits;
    if (cell == 0xffff) {
        *out = false;
        return;
    }
    UnkShockDirFilter_l9f6750 filter;
    filter.mode = 0;
    filter.min_cos = -1.0f;
    filter.dir.x = dir->x;
    filter.dir.y = dir->y;
    filter.field_0x10 = 0.0f;
    UnkNavSideFilter any_side = {};
    UnkNavSideVerdict no_verdict = {};
    if (!fn_710027b780(mesh, point, 0, 0, 0, 0, reinterpret_cast<UnkDirectionFilter*>(&filter),
                       reinterpret_cast<UnkNavMeshHits*>(&hits), any_side, no_verdict)) {
        *out = false;
        return;
    }
    if (hits.count == 0) {
        fn_7100287598(reinterpret_cast<UnkNavMeshHit*>(&hits.items[0]));
        hits.count = 1;
    }
    u8 side = hits.items[0].side;
    u8 front_side = side;
    u8 back_side = side;
    f32 reach = self->reach;
    CVector2D from = *start;
    UnkNavMeshCell* floor = fn_710027a328(mesh, cell);
    UnkNavMeshSideSegment edge = {};
    edge.field_0x10 = -1.0f;
    fn_71002afea8(floor, &side, &edge);
    if (self->floor_only && Dot_l9f6750(fn_7100125e78(reinterpret_cast<UnkSegment2D*>(&edge)), CVector2D{0.0f, 1.0f}) < 0.7f) {
        *out = false;
        return;
    }
    if (fn_71002afc84(floor, &side, self->cross_flags)) {
        *out = false;
        return;
    }
    // Half the reach each way: forward along the sides (towards each side's end), then back.
    f32 half = reach * 0.5f;
    f32 front_left = half;
    CVector2D front_from = from;
    u16 front_cell = cell;
    {
        bool walking = true;
        while (walking && front_left > 0.0f) {
            UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a328(mesh, front_cell), &front_side);
            UnkSegment2D edge(*reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, ends.a)),
                              *reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, ends.b)));
            f32 length = Distance_l9f6750(edge.end, front_from);
            if (length >= front_left)
                break;
            front_left -= length;
            u8 next_side;
            u16 next_cell;
            {
                CVector2D widths = {100.0f, 100.0f};
                if (!fn_710027c460(mesh, front_cell, front_side, true, &widths, 0, &next_cell, &next_side))
                    continue;
            }
            front_cell = next_cell;
            front_side = next_side;
            UnkNavMeshSidePair next_ends = fn_71002afe60(fn_710027a328(mesh, front_cell), &front_side);
            UnkSegment2D next(*reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, next_ends.a)),
                              *reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, next_ends.b)));
            front_from = next.start;
            if (Dot_l9f6750(fn_7100125e78(&next), *dir) > 0.99f)
                continue;
            if (front_left > 0.0f) {
                self->front_end = UnkVec3f(edge.end.x, edge.end.y, 0.0f);
                if (self->offset > 0.0f) {
                    UnkVec2Result out = fn_7100125e78(&edge);
                    f32 offset = self->offset;
                    UnkVec3f push(offset * out.x, offset * out.y, offset * 0.0f);
                    self->front_end.x = push.x + self->front_end.x;
                    self->front_end.y = push.y + self->front_end.y;
                    self->front_end.z = push.z + self->front_end.z;
                }
                self->front_found = true;
            }
            walking = false;
        }
    }
    {
        bool walking = true;
        while (walking && half > 0.0f) {
            UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a328(mesh, cell), &back_side);
            UnkSegment2D edge(*reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, ends.a)),
                              *reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, ends.b)));
            f32 length = Distance_l9f6750(edge.start, from);
            if (length >= half)
                break;
            half -= length;
            u8 next_side;
            u16 next_cell;
            {
                CVector2D widths = {100.0f, 100.0f};
                if (!fn_710027c460(mesh, cell, back_side, false, &widths, 0, &next_cell, &next_side))
                    continue;
            }
            cell = next_cell;
            back_side = next_side;
            UnkNavMeshSidePair next_ends = fn_71002afe60(fn_710027a328(mesh, cell), &back_side);
            UnkSegment2D next(*reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, next_ends.a)),
                              *reinterpret_cast<CVector2D*>(CNavMesh_GetVertex(mesh, next_ends.b)));
            from = next.end;
            if (Dot_l9f6750(fn_7100125e78(&next), *dir) > 0.99f)
                continue;
            if (half > 0.0f) {
                self->back_end = UnkVec3f(edge.start.x, edge.start.y, 0.0f);
                if (self->offset > 0.0f) {
                    UnkVec2Result out = fn_7100125e78(&edge);
                    f32 offset = self->offset;
                    UnkVec3f push(offset * out.x, offset * out.y, offset * 0.0f);
                    self->back_end.x = push.x + self->back_end.x;
                    self->back_end.y = push.y + self->back_end.y;
                    self->back_end.z = push.z + self->back_end.z;
                }
                self->back_found = true;
            }
            walking = false;
        }
    }
}
