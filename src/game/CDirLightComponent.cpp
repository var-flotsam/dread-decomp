// From unit 1388 of tools/tu_map.py (0x71008ea2cc-0x71008ebee0, CDirLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100143b3c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8; the layout of base/UnkHashMap.h). The same helper as
// src/game/COmniLightComponent.cpp's fn_71008f2444 for a smaller object. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// The scene object's part at +0x20 with a virtual update (as COmniLightComponent.cpp's).
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
struct UnkDirLightResRef {
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
struct UnkDirLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkDirLightObject {
    void* vtable;
    UnkDirLightBase base;          // 0x8
    UnkDirLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;             // 0x20
    u8 pad_0x28[0x70];
    UnkDirLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};
static_assert(sizeof(UnkDirLightObject) == 0x140, "UnkDirLightObject layout");

// Its constructor (the second argument is passed through).
void fn_7100143b3c(UnkDirLightObject* self, const void* arg);

struct UnkDirLightMapNode {
    UnkDirLightBase* key;
    UnkDirLightObject* value;
    UnkDirLightMapNode* next;   // the list of all nodes
    UnkDirLightMapNode* prev;
    UnkDirLightMapNode* chain;  // the next node in the bucket

    UnkDirLightMapNode(UnkDirLightBase* k, UnkDirLightObject* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the map's list ends).
    UnkDirLightMapNode*& Next() { return next; }
    UnkDirLightMapNode*& Prev() { return prev; }
};

struct UnkDirLightMap {
    base::UnkAllocator* allocator;
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkDirLightMapNode** buckets;   // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkDirLightMapNode* tail;  // 0x38
    UnkDirLightMapNode* head;  // 0x40

    // Adds a node at the front of the key's bucket (the key's low 32 bits modulo the capacity;
    // none, dereferenced all the same, for an empty bucket array) and at the end of the list.
    void Insert(UnkDirLightBase* key, UnkDirLightObject* value) {
        UnkDirLightMapNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)(u64)key % bucket_capacity;
            bucket = buckets + i;
        }
        UnkDirLightMapNode* old = *bucket;
        auto* node = static_cast<UnkDirLightMapNode*>(allocator->vf_0x40(sizeof(UnkDirLightMapNode), 8, flags));
        if (node)
            new (node) UnkDirLightMapNode(key, value);
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
UnkDirLightBase* fn_71008ebca8(UnkDirLightMap* map, UnkCountedResource* resource, UnkDirLightObject** out,
                               const void* arg) {
    auto* obj = static_cast<UnkDirLightObject*>(fn_71000839a0(sizeof(UnkDirLightObject), 0x38));
    fn_7100143b3c(obj, arg);
    UnkDirLightBase* base = nullptr;
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
