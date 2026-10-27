// From unit 1392 of tools/tu_map.py (0x71008f52c0-0x71008fab30, CSpotLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100144bac), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8). The same helper as src/game/CDirLightComponent.cpp's fn_71008ebca8
// (the same object layout) and src/game/COmniLightComponent.cpp's fn_71008f2444. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// The scene object's part at +0x20 with a virtual update (as the other light components').
struct UnkLightNode {
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

// Passes the part's object on (fn_71001bd9f0).
void fn_710020c634(UnkLightNode* self);

// A counted resource reference (base/UnkCountedResource.h).
struct UnkSpotLightResRef {
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
struct UnkSpotLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkSpotLightObject {
    void* vtable;
    UnkSpotLightBase base;          // 0x8
    UnkSpotLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;              // 0x20
    u8 pad_0x28[0x70];
    UnkSpotLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};
static_assert(sizeof(UnkSpotLightObject) == 0x140, "UnkSpotLightObject layout");

// Its constructor (the second argument is passed through).
void fn_7100144bac(UnkSpotLightObject* self, const void* arg);

struct UnkSpotLightMapNode {
    UnkSpotLightBase* key;
    UnkSpotLightObject* value;
    UnkSpotLightMapNode* next;   // the list of all nodes
    UnkSpotLightMapNode* prev;
    UnkSpotLightMapNode* chain;  // the next node in the bucket

    UnkSpotLightMapNode(UnkSpotLightBase* k, UnkSpotLightObject* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the map's list ends).
    UnkSpotLightMapNode*& Next() { return next; }
    UnkSpotLightMapNode*& Prev() { return prev; }
};

struct UnkSpotLightMap {
    base::UnkAllocator* allocator;
    u32 flags;                       // 0x8
    u32 count;                       // 0xc
    UnkSpotLightMapNode** buckets;   // 0x10
    u32 bucket_count;                // 0x18
    u32 bucket_capacity;             // 0x1c
    u8 pad_0x20[0x18];
    UnkSpotLightMapNode* tail;  // 0x38
    UnkSpotLightMapNode* head;  // 0x40

    // Adds a node at the front of the key's bucket (the key's low 32 bits modulo the capacity;
    // none, dereferenced all the same, for an empty bucket array) and at the end of the list.
    void Insert(UnkSpotLightBase* key, UnkSpotLightObject* value) {
        UnkSpotLightMapNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)(u64)key % bucket_capacity;
            bucket = buckets + i;
        }
        UnkSpotLightMapNode* old = *bucket;
        auto* node = static_cast<UnkSpotLightMapNode*>(allocator->vf_0x40(sizeof(UnkSpotLightMapNode), 8, flags));
        if (node)
            new (node) UnkSpotLightMapNode(key, value);
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
UnkSpotLightBase* fn_71008fa8f8(UnkSpotLightMap* map, UnkCountedResource* resource, UnkSpotLightObject** out,
                                const void* arg) {
    auto* obj = static_cast<UnkSpotLightObject*>(fn_71000839a0(sizeof(UnkSpotLightObject), 0x38));
    fn_7100144bac(obj, arg);
    UnkSpotLightBase* base = nullptr;
    if (obj) {
        UnkLightNode* node = &obj->node;
        fn_710020c634(node);
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
