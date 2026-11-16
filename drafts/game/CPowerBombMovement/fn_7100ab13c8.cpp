// CPowerBombMovement's unit (tools/tu_map.py unit 1612): tests the bomb's shape (g_unk_710164b850 scaled by
// size) at position against each collider in the movement's list (+0x530) and hands every hit to the bomb
// hit handler (fn_7100aaaaf4), tiles to the movement's vf_0x2b0. Names and layouts are placeholders.
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/UnkTU54.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a956b8[];
extern const u8 g_unk_710164b850[] __attribute__((visibility("hidden")));

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

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

static inline void ReleaseRef_lab13c8(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkPbHitRecord_lab13c8 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a23c58): the records and the collision components among them.
struct UnkPbHitList_lab13c8 {
    void** vtable;
    u32 count;
    u8 storage[64 * sizeof(UnkPbHitRecord_lab13c8)];  // 0xc
    u32 pad_0x120c;
    u32 componentCount;  // 0x1210
    u8 componentStorage[64 * sizeof(void*)];

    UnkPbHitRecord_lab13c8* items() { return reinterpret_cast<UnkPbHitRecord_lab13c8*>(storage); }

    UnkPbHitList_lab13c8() {
        vtable = g_unk_7101a23c58 + 2;
        count = 0;
        componentCount = 0;
    }
    ~UnkPbHitList_lab13c8() {
        componentCount = 0;
        vtable = g_unk_7101a23c58 + 2;
        Clear();
    }
    void Clear() {
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_lab13c8(items()[i].second);
            ReleaseRef_lab13c8(items()[i].first);
        }
        count = 0;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkPbHitRecord_lab13c8) == 0x48, "record");
static_assert(__builtin_offsetof(UnkPbHitList_lab13c8, componentCount) == 0x1210, "list");

// The query's visitor (vtable g_unk_7101a956b8).
struct UnkPbHitVisitor_lab13c8 : UnkPbHitList_lab13c8 {
    UnkPbHitVisitor_lab13c8() { vtable = g_unk_7101a956b8 + 2; }
};

struct UnkPbColliderOwner_lab13c8 {
    u8 pad_0x0[0xd0];
    void* collider;  // 0xd0
};

struct UnkPbColliderLink_lab13c8 {
    u8 pad_0x0[0x10];
    UnkPbColliderOwner_lab13c8* owner;  // 0x10
};

struct UnkPbColliderNode_lab13c8 {
    u8 pad_0x0[0x8];
    UnkPbColliderLink_lab13c8* link;  // 0x8
    UnkPbColliderNode_lab13c8* next;  // 0x10
};

// The power bomb's movement as the hit handler sees it; slot 0x2b0 takes the tiles it hits.
struct UnkPbMovement_lab13c8 : UnkBombHitHandler_ovb0078 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78();
    virtual void vf_0x80(); virtual void vf_0x88(); virtual void vf_0x90(); virtual void vf_0x98();
    virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8();
    virtual void vf_0xe0(); virtual void vf_0xe8(); virtual void vf_0xf0(); virtual void vf_0xf8();
    virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138();
    virtual void vf_0x140(); virtual void vf_0x148(); virtual void vf_0x150(); virtual void vf_0x158();
    virtual void vf_0x160(); virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178();
    virtual void vf_0x180(); virtual void vf_0x188(); virtual void vf_0x190(); virtual void vf_0x198();
    virtual void vf_0x1a0(); virtual void vf_0x1a8(); virtual void vf_0x1b0(); virtual void vf_0x1b8();
    virtual void vf_0x1c0(); virtual void vf_0x1c8(); virtual void vf_0x1d0(); virtual void vf_0x1d8();
    virtual void vf_0x1e0(); virtual void vf_0x1e8(); virtual void vf_0x1f0(); virtual void vf_0x1f8();
    virtual void vf_0x200(); virtual void vf_0x208(); virtual void vf_0x210(); virtual void vf_0x218();
    virtual void vf_0x220(); virtual void vf_0x228(); virtual void vf_0x230(); virtual void vf_0x238();
    virtual void vf_0x240(); virtual void vf_0x248(); virtual void vf_0x250(); virtual void vf_0x258();
    virtual void vf_0x260(); virtual void vf_0x268(); virtual void vf_0x270(); virtual void vf_0x278();
    virtual void vf_0x280(); virtual void vf_0x288(); virtual void vf_0x290(); virtual void vf_0x298();
    virtual void vf_0x2a0(); virtual void vf_0x2a8();
    virtual void vf_0x2b0(UnkBombHitObject_ovb0078* tiles, void* arg, const u64* key, UnkBombHitInfo_ovb0078* info,
                          f32 value);

    u8 pad_0x8[0x528];
    UnkPbColliderNode_lab13c8* colliders;  // 0x530
};
static_assert(__builtin_offsetof(UnkPbMovement_lab13c8, colliders) == 0x530, "movement");

void fn_7100ab13c8(UnkPbMovement_lab13c8* self, const UnkVec3f& position, f32 size) {
    UnkPbHitVisitor_lab13c8 hits;
    UnkLavaDropsShape_A2_b71 shape(g_unk_710164b850, size);
    for (UnkPbColliderNode_lab13c8* node = self->colliders; node; node = node->next) {
        UnkPbColliderLink_lab13c8* link = node->link;
        if (!link || !link->owner)
            continue;
        void* collider = link->owner->collider;
        UnkMatrix44_7100078c9c xf = fn_7100078cd8(position);
        if (!fn_7100d57ca0(&shape, &xf, collider, &hits, 0))
            continue;
        for (UnkPbHitRecord_lab13c8 *it = hits.items(), *end = it + hits.count; it != end; ++it)
            fn_7100aaaaf4(self, nullptr, const_cast<UnkVec3f*>(&position), reinterpret_cast<UnkBombHitInfo_ovb0078*>(it),
                          size, nullptr, static_cast<UnkBombOnTiles_ovb0078>(&UnkPbMovement_lab13c8::vf_0x2b0));
        hits.Clear();
    }
}