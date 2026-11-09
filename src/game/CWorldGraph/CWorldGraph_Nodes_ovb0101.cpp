// CWorldGraph's unit (tools/tu_map.py): the graph's nodes (0x80-byte objects whose constructors,
// move and destructor are out of line: fn_7100acf8d4 builds one at a point and navigation-mesh cell,
// fn_7100acd4cc default-constructs, fn_7100acf988 moves, fn_7100ad04c4 destroys) in the array at
// +0x30; the accessor that grows the array (fn_7100acc418), the insertion (fn_7100acd2a0), adding and
// erasing nodes while telling the listeners (the 0xd0-byte values of the keyed map at +0x16a8), the
// lookup by id that adds a node when there is none, and the destructor. Names and layouts are
// placeholders.
// The array's element accessor is inlined here (base/UnkVector.h).
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkComponent.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkWorldGraphArray;
void* fn_7100acc418(UnkWorldGraphArray* array, u32 index);
void fn_7100ad04c4(void* self);
void fn_7100acf988(void* self, void* other);
void fn_7100acd4cc(void* self);
void fn_710024f6d4(UnkBaseComponent* self);

// A node (0x80 bytes); its constructors and destructor are out of line, its id is read through
// UnkWorldGraphNodeData_b0101.
struct UnkWorldGraphNode_b0101 {
    u64 unk_0x0[0x10];

    UnkWorldGraphNode_b0101() { fn_7100acd4cc(this); }
    UnkWorldGraphNode_b0101(UnkWorldGraphNode_b0101&& other) { fn_7100acf988(this, &other); }
    ~UnkWorldGraphNode_b0101() { fn_7100ad04c4(this); }
};

struct UnkWorldGraphNodeData_b0101 {
    u64 field_0x0;
    u64 id;  // 0x8
    u8 pad_0x10[0x70];
};
static_assert(sizeof(UnkWorldGraphNodeData_b0101) == 0x80, "UnkWorldGraphNodeData_b0101 layout");

// A listener (0xd0 bytes): a value of the keyed map at +0x16a8.
struct UnkWorldGraphListener_b0101 {
    u8 unk_0x0[0xd0];
};
void fn_71006fae70(UnkWorldGraphListener_b0101* self, void* node, u32 index);
void fn_71006fadf0(UnkWorldGraphListener_b0101* self, void* node, u32 index);

struct UnkWorldGraph_b0101 {
    u8 pad_0x0[0x30];
    base::UnkVector<UnkWorldGraphNode_b0101> nodes;  // 0x30
    u8 pad_0x58[0x1650];
    UnkWorldGraphListener_b0101* listeners;  // 0x16a8: the map's values
    u64 field_0x16b0;                        // the map's keys
    u32 listener_count;                      // 0x16b8
};
static_assert(__builtin_offsetof(UnkWorldGraph_b0101, listeners) == 0x16a8, "UnkWorldGraph_b0101 layout");

// Erases the node at `index`, telling the listeners first.
void fn_7100acc92c(UnkWorldGraph_b0101* self, u32 index) {
    void* node = fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(&self->nodes), index);
    for (UnkWorldGraphListener_b0101 *it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        fn_71006fae70(it, node, index);
    base::UnkVector<UnkWorldGraphNode_b0101>* v = &self->nodes;
    if (index < v->count) {
        u64 i = index;
        v->data[i].~UnkWorldGraphNode_b0101();
        for (; i < (u32)(v->count - 1); ++i) {
            UnkWorldGraphNode_b0101* cur = &v->data[i];
            new (cur) UnkWorldGraphNode_b0101(static_cast<UnkWorldGraphNode_b0101&&>(cur[1]));
            (&v->data[i])[1].~UnkWorldGraphNode_b0101();
        }
        v->count = v->count - 1;
    }
}

// The node at `index`, growing the array to hold it.
__attribute__((noinline)) void* fn_7100acc418(UnkWorldGraphArray* array, u32 index) {
    return &reinterpret_cast<base::UnkVector<UnkWorldGraphNode_b0101>*>(array)->GrowAt(index);
}

// Moves `node` in at `index`.
__attribute__((flatten)) void fn_7100acd2a0(base::UnkVector<UnkWorldGraphNode_b0101>* nodes, UnkWorldGraphNode_b0101* node, u32 index) {
    nodes->Insert(static_cast<UnkWorldGraphNode_b0101&&>(*node), index);
}

void fn_7100acf8d4(void* self, const base::math::CVector2D* pos, void* arg, UnkNavMeshCell* cell, UnkWorldGraph_b0101* graph);

static inline UnkNavMeshManager* NavManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Inserts a node for `pos` (and the navigation mesh cell under it) at `index`, telling the
// listeners about the last node.
void* fn_7100acc2f0(UnkWorldGraph_b0101* self, u32 index, const base::math::CVector3D* pos, void* arg) {
    base::math::CVector2D point;
    fn_710007b430(&point, pos);
    UnkNavMesh* mesh = NavManager()->navmesh;
    u16 cell_index = fn_710027a41c(mesh, &point, 1);
    UnkNavMeshCell* cell = cell_index != 0xffff ? fn_710027a328(mesh, cell_index) : nullptr;
    alignas(8) u8 node[0x80];
    base::UnkVector<UnkWorldGraphNode_b0101>* nodes = &self->nodes;
    fn_7100acf8d4(node, &point, arg, cell, self);
    fn_7100acd2a0(nodes, reinterpret_cast<UnkWorldGraphNode_b0101*>(node), index);
    fn_7100ad04c4(node);
    void* last = fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(nodes), self->nodes.count - 1);
    for (UnkWorldGraphListener_b0101 *it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        fn_71006fadf0(it, last, self->nodes.count - 1);
    return last;
}

// The node whose id is `*key`; when there is none, a new node at `point` (none off the navigation
// mesh), appended and announced to the listeners.
__attribute__((disable_tail_calls)) void* fn_7100acc650(UnkWorldGraph_b0101* self, u64* key, const base::math::CVector2D* point) {
    base::UnkVector<UnkWorldGraphNode_b0101>* nodes = &self->nodes;
    for (u32 i = 0; i < self->nodes.count; ++i) {
        auto* node = static_cast<UnkWorldGraphNodeData_b0101*>(fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(nodes), i));
        if (node->id == *key)
            return fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(nodes), i);
    }
    UnkNavMesh* mesh = NavManager()->navmesh;
    u16 cell_index = fn_710027a41c(mesh, point, 1);
    if (cell_index == 0xffff)
        return nullptr;
    UnkNavMeshCell* cell = fn_710027a328(mesh, cell_index);
    alignas(8) u8 node[0x80];
    fn_7100acf8d4(node, point, key, cell, self);
    nodes->EmplaceBack(static_cast<UnkWorldGraphNode_b0101&&>(*reinterpret_cast<UnkWorldGraphNode_b0101*>(node)));
    fn_7100ad04c4(node);
    void* last = fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(nodes), self->nodes.count - 1);
    for (UnkWorldGraphListener_b0101 *it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        fn_71006fadf0(it, last, self->nodes.count - 1);
    return last;
}

// ---- The destructor ----

extern void* g_unk_7101a974b8[];  // the class vtable

// An element with a virtual destructor (its slot 0).
struct UnkWorldGraphPoly_b0101 {
    virtual ~UnkWorldGraphPoly_b0101();
};

// A fixed array of 100 0x20-byte polymorphic elements, its count first (4-byte aligned).
static inline void DestroyFixed(u8* base) {
    u32* count = reinterpret_cast<u32*>(base);
    for (u64 i = 0; i < *count; ++i)
        reinterpret_cast<UnkWorldGraphPoly_b0101*>(base + 4 + i * 0x20)->~UnkWorldGraphPoly_b0101();
    *count = 0;
}

struct UnkWorldGraphValue_b0101 {
    virtual ~UnkWorldGraphValue_b0101();
    u8 pad_0x8[0xc8];
};

struct UnkWorldGraphMap_b0101 {
    UnkWorldGraphValue_b0101* values;  // 0x16a8
    base::global::CStrId* keys;        // 0x16b0
    u32 count;                         // 0x16b8
    u32 capacity;
    u64 field_0x16c0;
    base::UnkAllocator* allocator;  // 0x16c8
};

struct UnkWorldGraphDtor_b0101 {
    void** vtable;
    u8 pad_0x8[0x28];
    base::UnkVector<UnkWorldGraphNode_b0101> nodes;  // 0x30
    u32 field_0x58;
    u8 pad_0x5c[0x320];
    u32 field_0x37c;
    u8 pad_0x380[0x320];
    u8 fixed_0x6a0[0xc84];
    u32 field_0x1324;
    u8 pad_0x1328[0x380];
    UnkWorldGraphMap_b0101 map;  // 0x16a8
    u32 field_0x16d0;
    u8 pad_0x16d4[0x320];
    u32 field_0x19f4;
    u8 pad_0x19f8[0x320];
    u8 fixed_0x1d18[0xc84];
    u32 field_0x299c;
};
static_assert(__builtin_offsetof(UnkWorldGraphDtor_b0101, map) == 0x16a8, "UnkWorldGraphDtor_b0101 layout");
static_assert(__builtin_offsetof(UnkWorldGraphDtor_b0101, field_0x299c) == 0x299c, "UnkWorldGraphDtor_b0101 layout");

// The destructor: the fixed arrays and the keyed map of listeners are cleared, the nodes destroyed and
// freed, then the component base destructor runs.
void fn_7100acc01c(UnkWorldGraphDtor_b0101* self) {
    self->vtable = g_unk_7101a974b8 + 2;
    self->field_0x299c = 0;
    DestroyFixed(self->fixed_0x1d18);
    self->field_0x19f4 = 0;
    self->field_0x16d0 = 0;
    UnkWorldGraphMap_b0101* map = &self->map;
    if (map->keys) {
        for (u64 i = 0; i < map->count; ++i)
            map->keys[i].Reset();
    }
    if (map->values) {
        for (u64 i = 0; i < map->count; ++i)
            map->values[i].~UnkWorldGraphValue_b0101();
    }
    map->count = 0;
    if (map->keys)
        map->allocator->vf_0x48(map->keys);
    if (map->values)
        map->allocator->vf_0x48(map->values);
    self->field_0x1324 = 0;
    DestroyFixed(self->fixed_0x6a0);
    self->field_0x37c = 0;
    self->field_0x58 = 0;
    self->nodes.Free();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}
