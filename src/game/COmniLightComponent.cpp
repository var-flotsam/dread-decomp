// From unit 1390 of tools/tu_map.py (0x71008efba0-0x71008f2680, COmniLightComponent by its
// reflection): the light component's helper that creates its 0x220-byte scene object (constructor
// fn_710014419c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8; the layout of base/UnkHashMap.h). Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// The scene object's part at +0x28 (constructed by fn_710020c38c) with a virtual update.
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

// Passes the part's object at +0x108 on (fn_71001bd9f0).
void fn_710020c634(UnkLightNode* self);

// A counted resource reference (base/UnkCountedResource.h), as src/game/UnkEnvironmentFade.cpp's.
struct UnkLightResRef {
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
struct UnkLightBase {
    u64 field_0x0;
};

// The 0x220-byte scene object.
struct UnkLightObject {
    void* vtable;
    UnkLightBase base;          // 0x8
    UnkLightResRef resource;    // 0x10
    u8 pad_0x18[0x10];
    UnkLightNode node;          // 0x28
    u8 pad_0x30[0x70];
    UnkLightResRef resource2;   // 0xa0
    u8 pad_0xa8[0x178];
};
static_assert(sizeof(UnkLightObject) == 0x220, "UnkLightObject layout");

// Its constructor (the second argument is passed through unused).
void fn_710014419c(UnkLightObject* self, const void* arg);

struct UnkLightMapNode {
    UnkLightBase* key;
    UnkLightObject* value;
    UnkLightMapNode* next;   // the list of all nodes
    UnkLightMapNode* prev;
    UnkLightMapNode* chain;  // the next node in the bucket

    UnkLightMapNode(UnkLightBase* k, UnkLightObject* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the map's list ends).
    UnkLightMapNode*& Next() { return next; }
    UnkLightMapNode*& Prev() { return prev; }
};

struct UnkLightMap {
    base::UnkAllocator* allocator;
    u32 flags;                   // 0x8
    u32 count;                   // 0xc
    UnkLightMapNode** buckets;   // 0x10
    u32 bucket_count;            // 0x18
    u32 bucket_capacity;         // 0x1c
    u8 pad_0x20[0x18];
    UnkLightMapNode* tail;  // 0x38
    UnkLightMapNode* head;  // 0x40

    // Adds a node at the front of the key's bucket (the key's low 32 bits modulo the capacity;
    // none, dereferenced all the same, for an empty bucket array) and at the end of the list.
    void Insert(UnkLightBase* key, UnkLightObject* value) {
        UnkLightMapNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)(u64)key % bucket_capacity;
            bucket = buckets + i;
        }
        UnkLightMapNode* old = *bucket;
        auto* node = static_cast<UnkLightMapNode*>(allocator->vf_0x40(sizeof(UnkLightMapNode), 8, flags));
        if (node)
            new (node) UnkLightMapNode(key, value);
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

// Creates a scene object, hands it the resource (also at +0xa0 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkLightBase* fn_71008f2444(UnkLightMap* map, UnkCountedResource* resource, UnkLightObject** out,
                            const void* arg) {
    auto* obj = static_cast<UnkLightObject*>(fn_71000839a0(sizeof(UnkLightObject), 0x38));
    fn_710014419c(obj, arg);
    UnkLightBase* base = nullptr;
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
