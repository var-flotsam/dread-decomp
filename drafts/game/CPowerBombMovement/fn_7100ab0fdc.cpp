// CPowerBombMovement's unit (tools/tu_map.py unit 1612): queries the collision tree (mask 0x80000) with the
// bomb's shape at its actor's position and records every breakable tile group it hits, once per id, in the
// movement's hashed list (a weak handle per group). Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern void* g_unk_7101a95690[];

struct UnkLavaDropsShape_A2_b71;
void fn_7100d6169c(UnkLavaDropsShape_A2_b71* self, const void* source, f32 size);
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

static inline void ReleaseRef_lab0fdc(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

// A breakable tile group as this unit reads it: its id at +0x10.
struct UnkPbTileGroup_lab0fdc : UnkReflObject {
    u8 pad_0x8[0x8];
    u32 id;  // 0x10
};

struct UnkPbHitShape_lab0fdc {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkPbHitEntry_lab0fdc : UnkRefObject {
    u8 pad_0xc[0x4];
    UnkPbHitShape_lab0fdc* shape;  // 0x10
};

#pragma pack(push, 4)
struct UnkPbHitRecord_lab0fdc {
    UnkPbHitEntry_lab0fdc* first;
    UnkRefObject* second;
    u8 pad_0x10[0x38];
};

// The hits of one query (vtable g_unk_7101a23c80): the records and the collision components among them.
struct UnkPbHitList_lab0fdc {
    void** vtable;
    u32 count;
    u8 storage[128 * sizeof(UnkPbHitRecord_lab0fdc)];  // 0xc
    u32 pad_0x240c;
    u32 componentCount;  // 0x2410
    u8 componentStorage[128 * sizeof(void*)];

    UnkPbHitRecord_lab0fdc* items() { return reinterpret_cast<UnkPbHitRecord_lab0fdc*>(storage); }

    UnkPbHitList_lab0fdc() {
        vtable = g_unk_7101a23c80 + 2;
        count = 0;
        componentCount = 0;
    }
    ~UnkPbHitList_lab0fdc() {
        componentCount = 0;
        vtable = g_unk_7101a23c80 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_lab0fdc(items()[i].second);
            ReleaseRef_lab0fdc(items()[i].first);
        }
        count = 0;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkPbHitRecord_lab0fdc) == 0x48, "record");
static_assert(__builtin_offsetof(UnkPbHitList_lab0fdc, componentCount) == 0x2410, "list");

// The query's visitor (vtable g_unk_7101a95690).
struct UnkPbHitVisitor_lab0fdc : UnkPbHitList_lab0fdc {
    UnkPbHitVisitor_lab0fdc() { vtable = g_unk_7101a95690 + 2; }
};

// A counted reference to a weak handle, held while it is stored.
struct UnkPbHandleRef_lab0fdc {
    UnkWeakHandle* handle;

    explicit UnkPbHandleRef_lab0fdc(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkPbHandleRef_lab0fdc() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot holding a counted reference to a weak handle.
struct UnkPbHandleSlot_lab0fdc {
    UnkWeakHandle* handle;

    UnkPbHandleSlot_lab0fdc() : handle(nullptr) {}
    void Assign(UnkWeakHandle* p) {
        if (p) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
};

// One tile group recorded: in its hash bucket's chain (+0x20) and in the list of all (+0x10, +0x18).
struct UnkPbTileNode_lab0fdc {
    u32 id;
    UnkPbHandleSlot_lab0fdc handle;  // 0x8
    UnkPbTileNode_lab0fdc* next;     // 0x10
    UnkPbTileNode_lab0fdc* prev;     // 0x18
    UnkPbTileNode_lab0fdc* chain;    // 0x20

    UnkPbTileNode_lab0fdc(u32 id_, UnkWeakHandle* h) : id(id_) {
        handle.Assign(h);
        next = nullptr;
        prev = nullptr;
        chain = nullptr;
    }
};
static_assert(sizeof(UnkPbTileNode_lab0fdc) == 0x28, "node");

struct UnkPbActor_lab0fdc {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkPowerBombMovement_lab0fdc {
    u8 pad_0x0[0x30];
    UnkPbActor_lab0fdc* actor;  // 0x30
    u8 pad_0x38[0x4f0 - 0x38];
    base::IAllocator* allocator;    // 0x4f0
    u32 allocFlags;                   // 0x4f8
    u32 tileCount;                    // 0x4fc
    UnkPbTileNode_lab0fdc** buckets;  // 0x500
    u32 field_0x508;
    u32 bucketCount;                  // 0x50c
    u8 pad_0x510[0x18];
    UnkPbTileNode_lab0fdc* tail;      // 0x528
    UnkPbTileNode_lab0fdc* head;      // 0x530

    UnkPbTileNode_lab0fdc** Bucket(u32 id) { return field_0x508 ? &buckets[id % bucketCount] : nullptr; }

    UnkPbTileNode_lab0fdc* Find(u32 id) {
        if (UnkPbTileNode_lab0fdc** bucket = Bucket(id)) {
            for (UnkPbTileNode_lab0fdc* node = *bucket; node; node = node->chain) {
                if (node->id == id)
                    return node;
            }
        }
        return nullptr;
    }

    void AddTile(u32 id, UnkWeakHandle* handle) {
        UnkPbTileNode_lab0fdc** bucket = Bucket(id);
        UnkPbTileNode_lab0fdc* first = *bucket;
        UnkPbTileNode_lab0fdc* node =
            new (allocator->vf_0x40(sizeof(UnkPbTileNode_lab0fdc), 8, allocFlags)) UnkPbTileNode_lab0fdc(id, handle);
        node->chain = first;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++tileCount;
    }
};
static_assert(__builtin_offsetof(UnkPowerBombMovement_lab0fdc, head) == 0x530, "movement");

// The shape is built around a point (a temporary that lives while the shape is built).
static inline const void* Around(const UnkVec3f& position) { return &position; }

void fn_7100ab0fdc(UnkPowerBombMovement_lab0fdc* self, f32 size) {
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (!tree)
        return;
    UnkPbHitVisitor_lab0fdc hits;
    bool hit;
    {
        UnkPbActor_lab0fdc* actor = self->actor;
        UnkLavaDropsShape_A2_b71 shape(Around(UnkVec3f(actor->x, actor->y, actor->z)), size);
        hit = QueryCollisionTreeForTransformedShapeOverlaps(tree, 0x80000, reinterpret_cast<UnkShape*>(&shape),
                            reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680), &hits, 0);
    }
    if (!hit)
        return;
    for (UnkPbHitRecord_lab0fdc *it = hits.items(), *end = it + hits.count; it != end; ++it) {
        UnkReflObject* object = it->first->shape->object;
        if (!object || !fn_710009d50c(object->vf_0x0(), fn_7100ca0484()))
            continue;
        u32 id = static_cast<UnkPbTileGroup_lab0fdc*>(object)->id;
        if (self->Find(id))
            continue;
        UnkPbHandleRef_lab0fdc handle(GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object)));
        self->AddTile(id, handle.handle);
    }
}
