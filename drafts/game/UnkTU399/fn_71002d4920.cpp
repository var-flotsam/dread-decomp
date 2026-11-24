// Unit 399 (tools/tu_map.py, animtree): the constructor of an animation tree instance from its definition.
// Builds the base and its tables, keeps a reference to the definition, creates the owned object, then walks
// the definition three times: creates its animated nodes (kept with their ids), its other nodes, and links
// the nodes its entries pair up; selects the definition's start entry. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkPairArrays.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

using base::global::CRntVector;

struct UnkNode;
struct UnkAnimTreeDefPtrs401;
struct UnkAnimTreeDef401;
struct UnkEntry401;
struct UnkAnimNode399;
struct UnkEntry399;
struct UnkNodeOwner_b0249;
struct UnkNode_b0249;
namespace base::reflection {
struct UnkReflObjectB;
}

extern void* g_unk_7101a21948[];
extern void* g_unk_7101a21748[];

// The object an animated node's +0x28 refers to.
struct UnkAnimTarget_l2d4920 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(UnkAnimTarget_l2d4920* other);
};

struct UnkAnimNode_l2d4920 {
    u8 pad_0x0[0x28];
    UnkAnimTarget_l2d4920* field_0x28;
};

struct UnkOwned_l2d4920 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
};

// A node of the definition.
struct UnkDefNodeInfo_l2d4920 {
    u8 pad_0x0[0x30];
    u64 field_0x30;  // the node's id
};
struct UnkDefNode_l2d4920 {
    u8 pad_0x0[0x8];
    UnkDefNodeInfo_l2d4920* info;
};

// An entry of the definition: the ids of two nodes.
struct UnkDefEntry_l2d4920 {
    u64 field_0x0;
    u64 field_0x8;
};

// A cursor over an array of the definition.
template <class T>
struct UnkDefWalk_l2d4920 {
    T* items;
    u32 count;
    u8 pad_0xc[0x1c];
    T* cursor;

    void Begin() { cursor = items; }
    void End() { cursor = items + count; }
};

// The definition's contents and the cursors that walk them.
struct UnkDefWalks_l2d4920 {
    u8 pad_0x0[0x10];
    u64 field_0x10;                                // the start entry's id
    UnkDefWalk_l2d4920<void*> nodes;               // 0x18
    UnkDefWalk_l2d4920<void*> others;              // 0x48
    UnkDefWalk_l2d4920<UnkDefEntry_l2d4920> links;  // 0x78
    u32 field_0xa8;
};

struct UnkAnimTreeDef_l2d4920 : UnkCountedResource {
    UnkDefWalks_l2d4920 walks;  // 0x58
};
static_assert(__builtin_offsetof(UnkAnimTreeDef_l2d4920, walks.field_0xa8) == 0x100, "layout");

// The nodes by id: two arrays sharing a count.
struct UnkNodeTable_l2d4920 {
    UnkAnimNode_l2d4920** values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkNodeTable_l2d4920(u32 flags)
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(10), flags(flags),
          allocator(GetDefaultAllocator()) {}

    UnkAnimNode_l2d4920** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key) {
                return &values[i];
            }
        }
        return values + count;
    }

    UnkAnimNode_l2d4920* Get(const u64& key) {
        UnkAnimNode_l2d4920** it = Find(key);
        return it == values + count ? nullptr : *it;
    }
};

void fn_71002d5af8(UnkNodeTable_l2d4920* self, u32 capacity);

struct UnkAnimTree_l2d4920 {
    void** vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;  // the definition's walks
    u8 pad_0x18[0x10];
    u64 field_0x28;
    u64 field_0x30;
    CRntVector<void*> field_0x38;
    f32 field_0x60;
    u32 field_0x64;
    u64 field_0x68;
    UnkOwned_l2d4920* owned;            // 0x70
    UnkNodeTable_l2d4920 nodes;         // 0x78
    CRntVector<void*> others;           // 0xa0
    base::UnkPairArrays animated;       // 0xc8: the animated nodes and their ids
    base::UnkPairArrays pair_b;         // 0xf0
    UnkCountedResource* definition;     // 0x118
};
static_assert(__builtin_offsetof(UnkAnimTree_l2d4920, definition) == 0x118, "layout");

void fn_71002dba08(UnkNode*);
void fn_71002d4e50(base::UnkPairArrays* self, u32 capacity, u32 flags);
void fn_710014b810(UnkOwned_l2d4920* self);
bool CAnimTreeDef_GetNextNode(UnkAnimTreeDefPtrs401* self, void** out);
bool CAnimTreeDef_NextControl(UnkAnimTreeDefPtrs401* self, void** out);
bool CAnimTreeDef_GetNextLink(UnkAnimTreeDef401* self, UnkEntry401** out);
UnkAnimNode_l2d4920* fn_71002d4f58(UnkAnimTree_l2d4920* self, void* node);
UnkNode_b0249* fn_71002d5070(UnkNodeOwner_b0249* self, base::reflection::UnkReflObjectB* def);
void fn_71002dfa18(UnkAnimNode399* self, UnkEntry399* entry);
void fn_71002d51d8(UnkAnimNode399* self, const u64& key);

// Adds a node and its id, growing both arrays by the step when they are full.
static inline void Add(base::UnkPairArrays& pairs, UnkAnimNode_l2d4920* node, u64 id) {
    if (pairs.count == pairs.capacity) {
        u32 capacity = pairs.count + pairs.step;
        if (pairs.count < capacity) {
            pairs.Grow(capacity);
            pairs.capacity = capacity;
        }
    }
    // Elements are copied bitwise, as Grow copies them.
    __builtin_memcpy(&pairs.values[pairs.count], &id, sizeof(u64));
    __builtin_memcpy(&pairs.keys[pairs.count], &node, sizeof(u64));
    ++pairs.count;
}

// Takes a reference to `resource` and drops the one `slot` held.
static inline void Assign(UnkCountedResource*& slot, UnkCountedResource* resource) {
    if (resource)
        ++resource->ref_count;
    if (UnkCountedResource* old = slot) {
        if (--old->ref_count == 0)
            fn_710001737c(old);
    }
    slot = resource;
}

void fn_71002d4920(UnkAnimTree_l2d4920* self, UnkAnimTreeDef_l2d4920* const& def) {
    fn_71002dba08(reinterpret_cast<UnkNode*>(self));
    self->vtable = g_unk_7101a21948 + 2;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    new (&self->field_0x38) CRntVector<void*>(0x40);
    self->field_0x38.Reserve(1);

    self->vtable = g_unk_7101a21748 + 2;
    self->field_0x60 = 0.0f;
    self->field_0x64 = 0;
    self->field_0x68 = 0;
    self->owned = nullptr;
    u32 node_count = def->walks.nodes.count;
    new (&self->nodes) UnkNodeTable_l2d4920(0x40);
    fn_71002d5af8(&self->nodes, node_count);
    u32 other_count = def->walks.others.count;
    new (&self->others) CRntVector<void*>(0xf);
    self->others.Reserve(other_count);
    fn_71002d4e50(&self->animated, def->walks.field_0xa8, 0x40);
    fn_71002d4e50(&self->pair_b, def->walks.field_0xa8, 0x40);
    self->definition = nullptr;
    Assign(self->definition, def);
    self->field_0x10 = &def->walks;

    UnkAnimTreeDef_l2d4920* d = def;
    UnkAnimTreeDefPtrs401* walks = reinterpret_cast<UnkAnimTreeDefPtrs401*>(&d->walks);
    UnkOwned_l2d4920* owned = static_cast<UnkOwned_l2d4920*>(fn_7100083b60(0x118));
    fn_710014b810(owned);
    UnkOwned_l2d4920* old = self->owned;
    self->owned = owned;
    if (old)
        old->Delete();

    d->walks.nodes.Begin();
    void* item;
    while (CAnimTreeDef_GetNextNode(walks, &item)) {
        UnkAnimNode_l2d4920* node = fn_71002d4f58(self, item);
        if (!fn_7100023d00(reinterpret_cast<const UnkCameraCtrl*>(node), fn_71002d2250()))
            continue;
        Add(self->animated, node, static_cast<UnkDefNode_l2d4920*>(item)->info->field_0x30);
    }
    d->walks.nodes.End();

    d->walks.others.Begin();
    while (CAnimTreeDef_NextControl(walks, &item))
        fn_71002d5070(reinterpret_cast<UnkNodeOwner_b0249*>(self), static_cast<base::reflection::UnkReflObjectB*>(item));
    d->walks.others.End();

    d->walks.links.Begin();
    UnkDefEntry_l2d4920* link;
    while (CAnimTreeDef_GetNextLink(reinterpret_cast<UnkAnimTreeDef401*>(walks), reinterpret_cast<UnkEntry401**>(&link))) {
        UnkAnimNode_l2d4920* to = self->nodes.Get(link->field_0x8);
        UnkAnimNode_l2d4920* from = self->nodes.Get(link->field_0x0);
        fn_71002dfa18(reinterpret_cast<UnkAnimNode399*>(to), reinterpret_cast<UnkEntry399*>(from));
        if (from->field_0x28 && to->field_0x28)
            to->field_0x28->vf_0x40(from->field_0x28);
    }
    d->walks.links.End();

    fn_71002d51d8(reinterpret_cast<UnkAnimNode399*>(self), d->walks.field_0x10);
    self->field_0x60 = 1.0f;
}
