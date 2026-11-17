// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): walks the navigation mesh's cell sides from the
// owner's position, one side after another in the given direction, until `budget` units are used up; false
// when the walk leaves the mesh or a side turns against the direction. Names and layouts are placeholders.
#include <math.h>

#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU362.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

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

// Up to 16 hits, destroyed with the list.
struct UnkInfesterHits_l5cc54c {
    UnkNavMeshHits list;
    u8 storage[16 * sizeof(UnkNavMeshHit)];

    UnkInfesterHits_l5cc54c() { list.count = 0; }
    ~UnkInfesterHits_l5cc54c() { list.Clear(); }
};

// From src/game/UnkDirectionFilter.cpp (the same definition).
struct UnkDirectionFilter {
    s32 mode;       // 2: compare with `direction`; otherwise with the up axis
    f32 threshold;  // -1 or below accepts every segment
    CVector3D direction;
};

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

struct Vec2;
using Mesh = UnkNavMesh;
Vec2* fn_710027a410(Mesh* mesh, u16 index);

// The next side along the mesh from side `side` of `cell`, in the given direction.
bool fn_710027c460(UnkNavMesh* mesh, u16 cell, u8 side, bool forward, const CVector2D* extents, u32 flags,
                   u16* out_cell, u8* out_side);
u32 fn_7100413fd0(UnkReflObject* ai);

static inline UnkNavMeshManager* UnkNavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline const CVector2D& Vertex(UnkNavMesh* mesh, u16 index) {
    return *reinterpret_cast<const CVector2D*>(fn_710027a410(mesh, index));
}

struct UnkInfesterAISlot_l5cc54c {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // +0x10
};

struct UnkInfesterOwner_l5cc54c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // +0x38; its translation column at +0x44, +0x54, +0x64
    u8 pad_0x78[0x240 - 0x78];
    UnkInfesterAISlot_l5cc54c* ai;  // +0x240
};

struct UnkInfesterSelf_l5cc54c {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_l5cc54c* owner;  // +0x30
};

bool fn_71005cc54c(UnkInfesterSelf_l5cc54c* self, bool forward, const CVector2D* goal, f32 budget) {
    UnkNavMeshManager* nav = UnkNavManager();
    CVector2D pos = {};
    {
        const UnkMatrix44& t = self->owner->transform;
        CVector3D at = {t.m[3], t.m[7], t.m[11]};
        fn_710007b444(&pos, reinterpret_cast<const UnkVec3*>(&at));
    }
    CVector2D facing;
    {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        CVector3D axis = {m.m[1], m.m[5], m.m[9]};
        fn_710007b430(&facing, &axis);
    }
    UnkNavMesh* mesh = UnkNavManager()->navmesh;
    u16 cell = fn_710027a41c(mesh, &pos, 0);

    UnkInfesterHits_l5cc54c hits;
    UnkDirectionFilter direction;
    direction.mode = 0;
    direction.threshold = -1.0f;
    direction.direction.x = 0.0f;
    direction.direction.y = 1.0f;
    direction.direction.z = 0.0f;
    UnkNavSideFilter filter = {};
    UnkNavSideVerdict verdict = {};
    if (!fn_710027b780(mesh, &pos, 0, 0, 0, 0, &direction, &hits.list, filter, verdict))
        return false;
    u8 side = hits.list.At(0).side;

    if (budget > 0.0f) {
        f32 gx = goal->x;
        f32 gy = goal->y;
        u16 current = cell;
        do {
            if (cell == 0xffff)
                return false;
            UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a33c(nav->navmesh, current), &side);
            UnkSegment2D edge(Vertex(nav->navmesh, ends.a), Vertex(nav->navmesh, ends.b));
            const CVector2D& p = forward ? edge.end : edge.start;
            f32 dx = p.x - gx;
            f32 dy = p.y - gy;
            f32 step = sqrtf(dx * dx + dy * dy);
            if (step >= budget)
                return true;

            UnkReflObject* o = self->owner->ai->component;
            UnkReflObject* ai = o && fn_710009d50c(o->vf_0x0(), fn_7100408b2c()) ? o : nullptr;
            u32 flags = fn_7100413fd0(ai);
            CVector2D extents = {100.0f, 100.0f};
            u8 next_side;
            u16 next_cell;
            if (fn_710027c460(nav->navmesh, current, side, forward, &extents, flags, &next_cell, &next_side)) {
                side = next_side;
                current = next_cell;
                UnkNavMeshSidePair next_ends = fn_71002afe60(fn_710027a33c(nav->navmesh, current), &side);
                UnkSegment2D next(Vertex(nav->navmesh, next_ends.a), Vertex(nav->navmesh, next_ends.b));
                f32 ex = next.end.x - next.start.x;
                f32 ey = next.end.y - next.start.y;
                const CVector2D& q = forward ? next.start : next.end;
                gx = q.x;
                gy = q.y;
                f32 length = sqrtf(ex * ex + ey * ey);
                if (length != 0.0f) {
                    f32 inv = 1.0f / length;
                    ex *= inv;
                    ey *= inv;
                }
                UnkVec2Result normal = fn_7100125e78(&edge);
                f32 d = ex * normal.x + ey * normal.y;
                bool turned = forward ? d > 0.0f : d < 0.0f;
                if (turned)
                    return false;
            }
            budget -= step;
        } while (budget > 0.0f);
    }
    return true;
}
