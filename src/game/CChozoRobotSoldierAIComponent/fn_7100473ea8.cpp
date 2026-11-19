// CChozoRobotSoldierAIComponent unit: the component's sides of a goal cell: the side nearest the goal
// through the navigation mesh (fn_710027b780, with the query's filter narrowed by fn_7100474f80 to sides
// at or below the goal), else the base's (fn_710110f318). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2702.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

using base::math::CVector2D;

template <class R, class... A>
struct UnkNavMeshCallback {
    void* ctx;
    R (*fn)(void* ctx, A... args);
};

struct UnkDirectionFilter;
struct UnkNavMeshSideSegment;
struct UnkNavMeshHits {
    u32 count;
};

using UnkNavSideFilter = UnkNavMeshCallback<bool, u16, u8, void*, void*, u64, u32, u32>;
using UnkNavSideVerdict = UnkNavMeshCallback<s32, UnkNavMeshSideSegment*>;

// A callback passed by value: copied member by member into memory.
template <class T>
struct UnkNavCallbackCopy_l473ea8 : T {
    UnkNavCallbackCopy_l473ea8(void* ctx_, decltype(T::fn) fn_) {
        this->ctx = ctx_;
        this->fn = fn_;
    }
    UnkNavCallbackCopy_l473ea8(const T& other) {
        this->ctx = other.ctx;
        this->fn = other.fn;
    }
};

struct UnkNavQuery_l473ea8 {
    u8 pad_0x0[0x10];
    UnkNavSideFilter filter;  // 0x10
    u8 pad_0x20[0x20];
    UnkNavSideVerdict verdict;  // 0x40
    u8 pad_0x50[0x10];
    u32 flags;  // 0x60
    u8 pad_0x64[0x4];
    u32 field_0x68;
    u32 field_0x6c;
    u8 pad_0x70[0x14];
    CVector2D goal;  // 0x84
    u8 pad_0x8c[0xd4];
    u8 direction[0x28];  // 0x160, UnkDirectionFilter
    u8 pad_0x188[0x8];
    u64 field_0x190;
};
static_assert(__builtin_offsetof(UnkNavQuery_l473ea8, field_0x190) == 0x190, "layout");

// What fn_7100474f80 receives: the query's own filter, the goal and the mesh.
struct UnkGoalFilter_l473ea8 {
    UnkNavSideFilter filter;
    CVector2D goal;
    UnkNavMesh* mesh = nullptr;
};

struct UnkYamplotNav_b0080;
struct UnkNavQuery_b0080;
struct UnkCellFilter_l474f80;
__attribute__((visibility("hidden"))) bool fn_7100474f80(UnkCellFilter_l474f80* self, u32 key, u8 side);

static inline UnkNavMeshManager* NavManager_l473ea8() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

void fn_7100473ea8(UnkYamplotNav_b0080* self, u16 cell, UnkNavQuery_l473ea8* query, UnkNavMeshHits* hits) {
    UnkGoalFilter_l473ea8 goal_filter;
    goal_filter.filter = query->filter;
    goal_filter.goal.x = query->goal.x;
    goal_filter.goal.y = query->goal.y;
    goal_filter.mesh = NavManager_l473ea8()->navmesh;
    if (goal_filter.mesh) {
        fn_710027b780(NavManager_l473ea8()->navmesh, &query->goal, query->flags, query->field_0x190,
                      query->field_0x6c, query->field_0x68, reinterpret_cast<UnkDirectionFilter*>(query->direction),
                      hits,
                      UnkNavCallbackCopy_l473ea8<UnkNavSideFilter>(
                          &goal_filter, reinterpret_cast<decltype(UnkNavSideFilter::fn)>(fn_7100474f80)),
                      UnkNavCallbackCopy_l473ea8<UnkNavSideVerdict>(query->verdict));
    }
    if (hits->count == 0)
        fn_710110f318(self, cell, reinterpret_cast<UnkNavQuery_b0080*>(query), hits);
}
