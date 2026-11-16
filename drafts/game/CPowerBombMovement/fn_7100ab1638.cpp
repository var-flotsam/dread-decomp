// CPowerBombMovement's unit (tools/tu_map.py unit 1612): queries the collision tree with a shape built from
// `source` and `size`, collecting up to 128 hits, and hands each hit to the bomb hit handler (fn_7100aaaaf4).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a95690[];

struct UnkLavaDropsShape_A2_b71;

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkBombHitObject_ovb0078;
struct UnkBombHitInfo_ovb0078;
struct UnkBombHitHandler_ovb0078 {};

using UnkBombOnActor_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(void* actor, void* arg, const u64* key,
                                                                   UnkBombHitInfo_ovb0078* info);
using UnkBombOnTiles_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(UnkBombHitObject_ovb0078* tiles, void* arg,
                                                                   const u64* key, UnkBombHitInfo_ovb0078* info,
                                                                   f32 value);

void fn_7100aaaaf4(UnkBombHitHandler_ovb0078* self, u32* kinds, void* arg, UnkBombHitInfo_ovb0078* info,
                   f32 value, UnkBombOnActor_ovb0078 on_actor, UnkBombOnTiles_ovb0078 on_tiles);

static inline void ReleaseRef_lab1638(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkPbHitRecord_lab1638 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a23c80): the records and the collision components among them.
struct UnkPbHitList_lab1638 {
    void** vtable;
    u32 count;
    u8 storage[128 * sizeof(UnkPbHitRecord_lab1638)];  // 0xc
    u32 pad_0x240c;
    u32 componentCount;  // 0x2410
    u8 componentStorage[128 * sizeof(void*)];

    UnkPbHitRecord_lab1638* items() { return reinterpret_cast<UnkPbHitRecord_lab1638*>(storage); }

    UnkPbHitList_lab1638() {
        vtable = g_unk_7101a23c80 + 2;
        count = 0;
        componentCount = 0;
    }
    ~UnkPbHitList_lab1638() {
        componentCount = 0;
        vtable = g_unk_7101a23c80 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_lab1638(items()[i].second);
            ReleaseRef_lab1638(items()[i].first);
        }
        count = 0;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkPbHitRecord_lab1638) == 0x48, "record");
static_assert(__builtin_offsetof(UnkPbHitList_lab1638, componentCount) == 0x2410, "list");

// The query's visitor (vtable g_unk_7101a95690).
struct UnkPbHitVisitor_lab1638 : UnkPbHitList_lab1638 {
    UnkPbHitVisitor_lab1638() { vtable = g_unk_7101a95690 + 2; }
};

void fn_7100ab1638(UnkBombHitHandler_ovb0078* self, u32 mask, const void* source, f32 size, u32* kinds,
                   UnkBombOnActor_ovb0078 on_actor, UnkBombOnTiles_ovb0078 on_tiles) {
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (!tree)
        return;
    UnkPbHitVisitor_lab1638 hits;
    bool hit;
    {
        UnkLavaDropsShape_A2_b71 shape(source, size);
        hit = fn_7100d5b484(tree, mask, reinterpret_cast<UnkShape*>(&shape),
                            reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680), &hits, 0);
    }
    if (hit) {
        for (UnkPbHitRecord_lab1638 *it = hits.items(), *end = it + hits.count; it != end; ++it)
            fn_7100aaaaf4(self, kinds, const_cast<void*>(source), reinterpret_cast<UnkBombHitInfo_ovb0078*>(it),
                          size, on_actor, on_tiles);
    }
}
