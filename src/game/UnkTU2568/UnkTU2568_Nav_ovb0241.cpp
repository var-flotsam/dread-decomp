// Unit 2568 of tools/tu_map.py (no reliable name): the navigation-mesh manager's registry of
// smart-link groups (game/UnkNavMeshManager.h). fn_7100ec1278 finds a group's slot by name and
// creates its first object on demand; fn_7100ec0dc4 finds the node of the group registered under
// a name (building each node's name id from its group); fn_7100ebf414 is the destructor of the
// node pool; fn_7100ec3068 grows the registry's parallel arrays (16-byte values, name ids) to a
// capacity; fn_7100ec1644 and fn_7100ec154c ask whether two cells (given as ids or as points)
// can reach each other; fn_7100ec1710 finds which of a list of candidate points is nearest to a goal
// by running a navigation query from each (creating the manager on first use). Names and layouts
// are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkParallelFind.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU357.h"
#include "game/UnkTU39.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;

// ---- The registry: parallel arrays of 16-byte values and the ids that name them ----

struct UnkNavGroupEntry_a1278 {
    void* first;
    void* second;
};
struct UnkNavGroupRegistry_a1278 {
    UnkNavMesh* navmesh;
    u8 pad_0x8[0x62e28];
    UnkNavGroupEntry_a1278* values;
    CStrId* ids;
    u32 count;
};

void fn_71002879ec(void* object, UnkNavMesh* mesh);

void* fn_7100ec1278(UnkNavMeshManager* self, const CStrId* name) {
    UnkNavGroupRegistry_a1278* r = reinterpret_cast<UnkNavGroupRegistry_a1278*>(self);
    UnkNavGroupEntry_a1278* slot = UnkParallelFindSlot(r, *name);
    if (slot == &r->values[r->count])
        return nullptr;
    if (slot->first == nullptr) {
        void* obj = fn_71000839a0(0x128, 0x57);
        fn_71002879ec(obj, r->navmesh);
        slot->first = obj;
    }
    return slot->first;
}

// ---- Looking a group up by name ----

// A string id built from a string (fn_7100000850), released in two steps.
struct UnkBuiltId_a0dc4 {
    union {
        CStrId id;
    };

    explicit UnkBuiltId_a0dc4(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId_a0dc4() { id.~CStrId(); }
};

struct UnkNavGroupName_a0dc4 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[0x4];
    // A union member: the original reloads the owner's pointer after the increment.
    union {
        s32 uses;  // 0x44
    };
};

struct UnkNavGroupNode_a0dc4 {
    u8 pad_0x0[0x8];
    base::reflection::UnkReflObject* group;  // 0x8
    UnkNavGroupNode_a0dc4* next;             // 0x10
};

struct UnkNavGroupList_a0dc4 {
    u8 pad_0x0[0x62e68];
    UnkNavGroupNode_a0dc4* head;
};

UnkNavGroupName_a0dc4** fn_7100266264(base::reflection::UnkReflObject* group);

void* fn_7100ec0dc4(UnkNavMeshManager* self, const CStrId* key) {
    UnkNavGroupNode_a0dc4* node = reinterpret_cast<UnkNavGroupList_a0dc4*>(self)->head;
    while (node) {
        base::reflection::UnkReflObject* obj = node->group;
        base::reflection::UnkReflObject* group = nullptr;
        if (obj)
            group = fn_710009d50c(obj->vf_0x0(), fn_7100264814()) ? obj : nullptr;
        UnkNavGroupName_a0dc4** holder = fn_7100266264(group);
        ++(*holder)->uses;
        base::global::UnkStringData* data;
        base::global::UnkStringData* wanted;
        {
            UnkBuiltId_a0dc4 built((*holder)->name);
            data = built.id.data;
            wanted = key->data;
        }
        if (data == wanted)
            break;
        node = node->next;
    }
    return node;
}

// ---- The node pool's destructor ----

extern char g_unk_7101ca9f70[];
extern void* g_unk_7101ca9f90[];

struct UnkNavNode_af414 {
    u8 pad_0x0[0x8];
    UnkCountedResource* resource;  // 0x8
    UnkNavNode_af414* next;        // 0x10
};

// The pool of registry nodes (fn_7100ebf2fc builds it): a node list over a block pool whose nodes
// hold a counted resource.
struct UnkNavNodePool_af414 {
    char* vtable;
    u32 count;
    UnkNavNode_af414* head;
    UnkNavNode_af414* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    base::UnkBlockPool pool;  // 0x30

    __attribute__((always_inline)) void FreeNodes() {
        for (UnkNavNode_af414* n = head; n;) {
            UnkNavNode_af414* next = n->next;
            if (UnkCountedResource* r = n->resource) {
                if (--r->ref_count == 0)
                    fn_710001737c(r);
            }
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

void fn_7100ebf414(UnkNavNodePool_af414* self) {
    self->vtable = g_unk_7101ca9f70 + 0x10;
    self->pool.field_0x3c = true;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = nullptr;
    self->pool.~UnkBlockPool();
    self->vtable = reinterpret_cast<char*>(g_unk_7101ca9f90) + 0x10;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

// ---- Growing the registry's arrays ----

using UnkIdMember_a3068 = base::UnkStrIdMember;

struct UnkNavPair_a3068 {
    u64 first;
    u64 second;
};

struct UnkNavPairArray_a3068 {
    UnkNavPair_a3068* values;
    base::UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100ec3068(UnkNavPairArray_a3068* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkNavPairArray_a3068* a = self;
    auto* v = static_cast<UnkNavPair_a3068*>(a->allocator->vf_0x40((u64)n * 16, 8, a->flags));
    auto* k = static_cast<base::UnkStrIdMember*>(a->allocator->vf_0x40((u64)n * 8, 8, a->flags));
    if (a->values) {
        UnkNavPair_a3068* src = a->values;
        UnkNavPair_a3068* dst = v;
        for (u32 i = 0; i < a->count; ++i, ++src, ++dst)
            *dst = *src;
        a->allocator->vf_0x48(a->values);
    }
    if (a->keys) {
        base::UnkStrIdMember* src = a->keys;
        base::UnkStrIdMember* dst = k;
        for (u32 i = 0; i < a->count; ++i, ++src, ++dst) {
            union Slot {
                base::global::UnkStringData* p;
            };
            dst->data = src->data;
            reinterpret_cast<Slot*>(&src->data)->p = &g_unk_7101d47108->empty;
            src->~UnkIdMember_a3068();
        }
        a->allocator->vf_0x48(a->keys);
    }
    a->values = v;
    a->keys = k;
    a->capacity = n;
}

// ---- Reachability between two cells ----

struct UnkNavCellData_a1644;
struct UnkNavCellOwner_a1644 {
    u8 pad_0x0[0x68];
    UnkNavCellData_a1644* data;  // 0x68
};

bool fn_71002b361c(UnkNavCellData_a1644* data, u32 flags);
bool fn_71002b497c(UnkNavMeshReach_0x5038* tester, UnkNavCellData_a1644* from, UnkNavCellData_a1644* to, u32 flags,
                   u32 extra);

bool fn_7100ec1644(UnkNavMeshManager* self, u16 a, u16 b, u32 flags, u32 extra) {
    if (b != 0xffff && self->navmesh && a != 0xffff) {
        UnkNavCellData_a1644* from = reinterpret_cast<UnkNavCellOwner_a1644*>(fn_710027a33c(self->navmesh, a))->data;
        UnkNavCellData_a1644* to = reinterpret_cast<UnkNavCellOwner_a1644*>(fn_710027a33c(self->navmesh, b))->data;
        if (from && to && fn_71002b361c(from, flags) && fn_71002b361c(to, flags))
            return fn_71002b497c(&self->reach, from, to, flags, extra);
    }
    return false;
}

// The same for two points: their cells are looked up first.
bool fn_7100ec154c(void* self_, const CVector2D* a, const CVector2D* b, u32 flags, u32 extra) {
    UnkNavMeshManager* self = static_cast<UnkNavMeshManager*>(self_);
    if (self->navmesh) {
        u16 from_id = fn_710027a41c(self->navmesh, a, 1);
        u16 to_id = fn_710027a41c(self->navmesh, b, 1);
        if (from_id != 0xffff && to_id != 0xffff && self->navmesh) {
            UnkNavCellData_a1644* from =
                reinterpret_cast<UnkNavCellOwner_a1644*>(fn_710027a33c(self->navmesh, from_id))->data;
            UnkNavCellData_a1644* to = reinterpret_cast<UnkNavCellOwner_a1644*>(fn_710027a33c(self->navmesh, to_id))->data;
            if (from && to && fn_71002b361c(from, flags) && fn_71002b361c(to, flags))
                return fn_71002b497c(&self->reach, from, to, flags, extra);
        }
    }
    return false;
}

// ---- Nearest candidate by navigation distance ----

struct UnkWeakHandle;
struct UnkNavCallback_b0194 {
    void* ctx;
    void* fn;
};

// The navigation query as the AI builds it: only the members set here are named.
struct UnkNavQuery {
    u8 pad_0x0[0x20];
    UnkNavCallback_b0194 filter;  // 0x20
    u8 pad_0x30[0x28];
    UnkWeakHandle* target;  // 0x58
    u32 flags;              // 0x60
    u8 pad_0x64[0xf0];
    u32 field_0x154;
    u8 pad_0x158[0x48];
    u8 field_0x1a0;
    u8 pad_0x1a1[3];
    f32 field_0x1a4;
    f32 field_0x1a8;
    u8 pad_0x1ac[0xc];
};
static_assert(sizeof(UnkNavQuery) == 0x1b8, "UnkNavQuery layout");

struct UnkNavMeshHits;
struct UnkStruct_7100286624;
struct UnkObject_71002ad0dc;

void fn_7100286920(UnkNavQuery* self, const CVector2D* point, u16 cell, u8 side);
void fn_7100286964(UnkNavQuery* self, const CVector2D* start, u16 cell, UnkNavMeshHits* hits);

struct UnkLocalQuery_ec1710 : UnkNavQuery {
    UnkLocalQuery_ec1710() { fn_710028659c(this); }
    ~UnkLocalQuery_ec1710() { fn_7100286624(reinterpret_cast<UnkStruct_7100286624*>(this)); }
};

struct UnkScratch_ec1710 {
    u8 data[0x12a88];
    UnkScratch_ec1710() { fn_71002acfe4(this); }
    ~UnkScratch_ec1710() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct UnkCandidate_ec1710 {
    u16 cell;
    u8 pad_0x2[2];
    CVector2D point;  // 0x4
    f32 distance;     // 0xc
};

struct UnkCandidates_ec1710 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
};

struct UnkCellData_ec1710;
struct UnkCellOwner_ec1710 {
    u8 pad_0x0[0x68];
    UnkCellData_ec1710* data;  // 0x68
};
struct UnkCellData_ec1710 {
    u8 pad_0x0[0x18];
    u8 field_0x18[1];
};

UnkCandidate_ec1710* fn_710054cdac(UnkCandidates_ec1710* list, u32 index);
void fn_7100ec1970(const CVector2D* start, UnkCellData_ec1710* data, base::UnkVector<void*>* tmp,
                   UnkCandidates_ec1710* out);
bool fn_71002811b4(UnkNavMeshState_0x5ddf0* state, UnkNavQuery* query, UnkScratch_ec1710* scratch);
f32 fn_71002aca24(void* result);

void fn_71002810b0(UnkNavMeshState_0x5ddf0* self);
void fn_71002b4934(UnkNavMeshReach_0x5038* self);
void fn_7100ebf2fc(UnkNavMeshNodePool_0x70* self, u32 count, u32 unused, u32 flags, base::UnkAllocator* allocator);

// The manager, created on first use with its constructor (fn_7100ebf278) inlined.
static inline UnkNavMeshManager* GetRegistry() {
    UnkNavMeshManager* registry = g_unk_71726a4510;
    if (!registry) {
        registry = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        registry->navmesh = nullptr;
        fn_71002810b0(&registry->state);
        fn_71002b4934(&registry->reach);
        registry->field_0x62e38 = nullptr;
        registry->field_0x62e40 = nullptr;
        registry->field_0x62e30 = nullptr;
        registry->field_0x62e48 = 10;
        registry->field_0x62e4c = 0x57;
        registry->allocator = fn_7100083990();
        fn_7100ebf2fc(&registry->pool, 100, 0, 0x57, nullptr);
        g_unk_71726a4510 = registry;
    }
    return registry;
}

void fn_7100ec1710(UnkNavMeshManager* self, const CVector2D* start, const CVector2D* goal, u16 cell, u32* best_index,
                   UnkCandidates_ec1710* candidates) {
    *best_index = 0xffffffff;
    if (cell == 0xffff)
        return;
    if (!self->navmesh)
        return;
    UnkCellData_ec1710* data = reinterpret_cast<UnkCellOwner_ec1710*>(fn_710027a33c(self->navmesh, cell))->data;
    if (!data)
        return;
    if (fn_710002fb70(data->field_0x18))
        return;
    base::UnkVector<void*> tmp(0xf);
    fn_7100ec1970(start, data, &tmp, candidates);
    f32 best = 3.4028235e38f;
    for (u32 i = 0; i < candidates->count; ++i) {
        UnkCandidate_ec1710* candidate = fn_710054cdac(candidates, i);
        UnkScratch_ec1710 scratch;
        UnkLocalQuery_ec1710 query;
        fn_7100286920(&query, goal, cell, 0xff);
        query.flags = 1;
        fn_7100286964(&query, &candidate->point, candidate->cell, nullptr);
        if (fn_71002811b4(&GetRegistry()->state, &query, &scratch)) {
            f32 d = fn_71002aca24(scratch.data + 0x10);
            candidate->distance = d;
            if (d < best) {
                best = d;
                *best_index = i;
            }
        }
    }
    tmp.Free();
}
