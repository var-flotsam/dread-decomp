// CBombMovement's unit (tools/tu_map.py unit 1610): queries the collision tree with a shape built from `source`
// and `size`, collecting up to 32 hits (the query also keeps the position), and hands each hit to the bomb hit
// handler (fn_7100aaaaf4). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CBombMovement.h"
#include "game/CDroppableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkLavaDropsShape_A2_b71.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a94fd8[];

struct UnkLavaDropsShape_A2_b71;

static inline void ReleaseRef_laaa914(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct UnkBombQueryPos_laaa914 {
    f32 x;
    f32 y;
    f32 z;
};

#pragma pack(push, 4)
struct UnkBombHitRecord_laaa914 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a70eb0) and the position it was made at.
struct UnkBombHitList_laaa914 {
    void** vtable;
    u32 count;
    u8 storage[32 * sizeof(UnkBombHitRecord_laaa914)];  // 0xc
    UnkBombQueryPos_laaa914 pos;                         // 0x90c

    UnkBombHitList_laaa914() : count(0) {}

    UnkBombHitRecord_laaa914* items() { return reinterpret_cast<UnkBombHitRecord_laaa914*>(storage); }

    ~UnkBombHitList_laaa914() {
        vtable = g_unk_7101a70eb0 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_laaa914(items()[i].second);
            ReleaseRef_laaa914(items()[i].first);
        }
        count = 0;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkBombHitRecord_laaa914) == 0x48, "record");
static_assert(__builtin_offsetof(UnkBombHitList_laaa914, pos) == 0x90c, "list");

// The query's visitor (vtable g_unk_7101a94fd8).
struct UnkBombHitVisitor_laaa914 : UnkBombHitList_laaa914 {
    UnkBombHitVisitor_laaa914(UnkBombQueryPos_laaa914 at) {
        vtable = g_unk_7101a94fd8 + 2;
        pos = at;
    }
};

void fn_7100aaa914(UnkBombHitHandler_ovb0078* self, u32 mask, const void* source, f32 size, u32* kinds,
                   UnkBombOnActor_ovb0078 on_actor, UnkBombOnTiles_ovb0078 on_tiles) {
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (!tree)
        return;
    UnkLavaDropsShape_A2_b71 shape(source, size);
    UnkBombHitVisitor_laaa914 hits(*static_cast<const UnkBombQueryPos_laaa914*>(source));
    if (fn_7100d5b484(tree, mask, reinterpret_cast<UnkShape*>(&shape),
                      reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680), &hits, 0)) {
        for (UnkBombHitRecord_laaa914* it = hits.items(); it != hits.items() + hits.count; ++it)
            CBombMovement_DispatchHitByClass(self, kinds, const_cast<void*>(source), reinterpret_cast<UnkBombHitInfo_ovb0078*>(it),
                          size, on_actor, on_tiles);
    }
}
