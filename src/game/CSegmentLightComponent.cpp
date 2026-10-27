// From unit 1391 of tools/tu_map.py (0x71008f2680-0x71008f52c0, CSegmentLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100142b8c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8). The same as COmniLightComponent's fn_71008f2444
// (src/game/COmniLightComponent.cpp) over a smaller object. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// The scene object's part at +0x20 with a virtual update.
struct UnkSegmentLightNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

// Passes the part's object at +0x108 on (fn_71001bd9f0). Declared by src/game/COmniLightComponent.cpp
// over its own node type.
struct UnkLightNode;
void fn_710020c634(UnkLightNode* self);

// A counted resource reference (base/UnkCountedResource.h).
struct UnkSegmentLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkSegmentLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkSegmentLightObject {
    void* vtable;
    UnkSegmentLightBase base;            // 0x8
    UnkSegmentLightResRef resource;      // 0x10
    u8 pad_0x18[0x8];
    UnkSegmentLightNode node;            // 0x20
    u8 pad_0x28[0x70];
    UnkSegmentLightResRef resource2;     // 0x98
    u8 pad_0xa0[0xa0];
};
static_assert(sizeof(UnkSegmentLightObject) == 0x140, "UnkSegmentLightObject layout");

// Its constructor (the second argument is the object's name).
void fn_7100142b8c(UnkSegmentLightObject* self, const void* arg);

struct UnkSegmentLightMapNode {
    UnkSegmentLightBase* key;
    UnkSegmentLightObject* value;
    UnkSegmentLightMapNode* next;   // the list of all nodes
    UnkSegmentLightMapNode* prev;
    UnkSegmentLightMapNode* chain;  // the next node in the bucket

    UnkSegmentLightMapNode(UnkSegmentLightBase* k, UnkSegmentLightObject* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the map's list ends).
    UnkSegmentLightMapNode*& Next() { return next; }
    UnkSegmentLightMapNode*& Prev() { return prev; }
};

struct UnkSegmentLightMap {
    base::UnkAllocator* allocator;
    u32 flags;                          // 0x8
    u32 count;                          // 0xc
    UnkSegmentLightMapNode** buckets;   // 0x10
    u32 bucket_count;                   // 0x18
    u32 bucket_capacity;                // 0x1c
    u8 pad_0x20[0x18];
    UnkSegmentLightMapNode* tail;       // 0x38
    UnkSegmentLightMapNode* head;       // 0x40

    // Adds a node at the front of the key's bucket (the key's low 32 bits modulo the capacity;
    // none, dereferenced all the same, for an empty bucket array) and at the end of the list.
    void Insert(UnkSegmentLightBase* key, UnkSegmentLightObject* value) {
        UnkSegmentLightMapNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)(u64)key % bucket_capacity;
            bucket = buckets + i;
        }
        UnkSegmentLightMapNode* old = *bucket;
        auto* node = static_cast<UnkSegmentLightMapNode*>(allocator->vf_0x40(sizeof(UnkSegmentLightMapNode), 8, flags));
        if (node)
            new (node) UnkSegmentLightMapNode(key, value);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

// Creates a scene object, hands it the resource (also at +0x98 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkSegmentLightBase* fn_71008f5084(UnkSegmentLightMap* map, UnkCountedResource* resource,
                                   UnkSegmentLightObject** out, const void* arg) {
    auto* obj = static_cast<UnkSegmentLightObject*>(fn_71000839a0(sizeof(UnkSegmentLightObject), 0x38));
    fn_7100142b8c(obj, arg);
    UnkSegmentLightBase* base = nullptr;
    if (obj) {
        UnkSegmentLightNode* node = &obj->node;
        fn_710020c634(reinterpret_cast<UnkLightNode*>(node));
        obj->resource.Set(resource);
        base = &obj->base;
        if (resource) {
            obj->resource2.Set(resource);
            node->vf_0x90();
        }
        map->Insert(base, obj);
    }
    if (out)
        *out = obj;
    return base;
}
