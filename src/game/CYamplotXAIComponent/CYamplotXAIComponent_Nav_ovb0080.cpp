// CYamplotXAIComponent's unit (tools/tu_map.py unit 1109): the component's sides of a goal cell (its
// vf_0x548, as src/game/UnkNavQuery.cpp calls it): the side nearest the goal through the navigation mesh
// (fn_710027b780, with the query's filter and this unit's verdict fn_71006f328c), else the base's
// (fn_710110f318). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
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

bool fn_710027b780(UnkNavMesh* mesh, const CVector2D* point, u32 flags, u64 filter_arg, u32 filter_arg2,
                   u32 filter_arg3, UnkDirectionFilter* direction, UnkNavMeshHits* out,
                   const UnkNavSideFilter& filter, const UnkNavSideVerdict& verdict);

// A callback passed by value: copied member by member into memory.
template <class T>
struct UnkNavCallbackCopy_b0080 : T {
    UnkNavCallbackCopy_b0080(void* ctx_, decltype(T::fn) fn_) {
        this->ctx = ctx_;
        this->fn = fn_;
    }
    UnkNavCallbackCopy_b0080(const T& other) {
        this->ctx = other.ctx;
        this->fn = other.fn;
    }
};

struct UnkNavQuery_b0080 {
    u8 pad_0x0[0x10];
    UnkNavSideFilter filter;  // 0x10
    u8 pad_0x20[0x40];
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
static_assert(__builtin_offsetof(UnkNavQuery_b0080, field_0x190) == 0x190, "layout");

struct UnkYamplotNav_b0080;
s32 fn_71006f328c(void* ctx, UnkNavMeshSideSegment* seg);
void fn_710110f318(UnkYamplotNav_b0080* self, u16 cell, UnkNavQuery_b0080* query, UnkNavMeshHits* hits);

static inline UnkNavMeshManager* NavManager_b0080() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

void fn_71006f0bc0(UnkYamplotNav_b0080* self, u16 cell, UnkNavQuery_b0080* query, UnkNavMeshHits* hits) {
    if (NavManager_b0080()->navmesh) {
        fn_710027b780(NavManager_b0080()->navmesh, &query->goal, query->flags, query->field_0x190, query->field_0x6c,
                      query->field_0x68, reinterpret_cast<UnkDirectionFilter*>(query->direction), hits,
                      UnkNavCallbackCopy_b0080<UnkNavSideFilter>(query->filter),
                      UnkNavCallbackCopy_b0080<UnkNavSideVerdict>(self, fn_71006f328c));
    }
    if (hits->count == 0)
        fn_710110f318(self, cell, query, hits);
}
