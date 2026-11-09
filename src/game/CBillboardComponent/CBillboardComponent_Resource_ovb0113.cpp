// CBillboardComponent's unit (tools/tu_map.py, unit 1152): the resource holder's load from a registered key
// (fn_710072a000), the part of UnkResHolderLoad (base/UnkResKey.h) after the key lookup, with the
// UnkResHolder2 layout: the resource the key names is kept in both holders and the notified object is told.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkResKey.h"
#include "base/UnkVector.h"
#include "types.h"

// A holder's assignment from a resource known to be there.
static inline void Assign(UnkResRef* dst, UnkCountedResource* p) {
    if (dst->ptr != p) {
        ++p->ref_count;
        UnkCountedResource* old = dst->ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        dst->ptr = p;
    }
}

bool fn_710072a000(UnkResHolder2* self, const UnkResKey* key) {

    UnkResRef resource = fn_71000ead80(g_unk_7101d472f8, key, self->flag, nullptr);
    UnkCountedResource* p = resource.ptr;
    if (!p)
        return false;
    Assign(&self->first, p);
    UnkResHolderNotified* notified = self->Notified();
    Assign(&self->second, p);
    notified->vf_0x90();
    return true;
}

// fn_7100729eac: the pool's creator for billboards (like fn_710021d3d4 in
// src/base/spatial/UnkModelInstanceSet.cpp): a 0x130-byte tagged widget object (fn_71001431ec) whose billboard
// (+0x120) is prepared (fn_71001b4de8) and, for a key with a path, given the key's resource; one that fails to
// load is destroyed (fn_7100143244) and freed. The rest are added to the pool's chained hash set and returned,
// also through `out` when given.
struct UnkTaggedWidgetObject;
struct UnkWidgetArg;
void fn_71001431ec(UnkTaggedWidgetObject* self, UnkWidgetArg* arg);
struct UnkTaggedObject;
void fn_7100143244(UnkTaggedObject* self);
struct UnkBillboard;
void fn_71001b4de8(UnkBillboard* self);

struct UnkBillboardObject_ovb0113 {
    u8 pad_0x0[0x120];
    UnkBillboard* billboard;  // 0x120
    u8 pad_0x128[0x8];
};
static_assert(sizeof(UnkBillboardObject_ovb0113) == 0x130, "UnkBillboardObject_ovb0113 layout");

struct UnkBillboardNode_ovb0113 {
    u64 key;
    u64 value;
    UnkBillboardNode_ovb0113* next;   // the list of all nodes
    UnkBillboardNode_ovb0113* prev;
    UnkBillboardNode_ovb0113* chain;  // the next node in the bucket

    explicit UnkBillboardNode_ovb0113(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkBillboardNode_ovb0113*& Next() { return next; }
    UnkBillboardNode_ovb0113*& Prev() { return prev; }
};

struct UnkBillboardSet_ovb0113 {
    base::UnkAllocator* allocator;
    u32 flags;                              // 0x8
    u32 count;                              // 0xc
    UnkBillboardNode_ovb0113** buckets;     // 0x10
    u32 bucket_count;                       // 0x18
    u32 bucket_capacity;                    // 0x1c
    u8 pad_0x20[0x18];
    UnkBillboardNode_ovb0113* tail;  // 0x38
    UnkBillboardNode_ovb0113* head;  // 0x40

    void Insert(u64 key) {
        UnkBillboardNode_ovb0113** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            bucket = &buckets[i];
        }
        UnkBillboardNode_ovb0113* old = *bucket;
        auto* node = static_cast<UnkBillboardNode_ovb0113*>(
            allocator->vf_0x40(sizeof(UnkBillboardNode_ovb0113), 8, flags));
        if (node)
            new (node) UnkBillboardNode_ovb0113(key);
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

UnkBillboardObject_ovb0113* fn_7100729eac(UnkBillboardSet_ovb0113* set, const UnkResKey* key,
                                          UnkBillboardObject_ovb0113** out, UnkWidgetArg* arg) {
    auto* object = static_cast<UnkBillboardObject_ovb0113*>(fn_71000839a0(sizeof(UnkBillboardObject_ovb0113), 0x38));
    fn_71001431ec(reinterpret_cast<UnkTaggedWidgetObject*>(object), arg);
    UnkBillboardObject_ovb0113* result = nullptr;
    if (object) {
        fn_71001b4de8(object->billboard);
        if (key->path.data != &g_unk_7101d47200->empty &&
            !fn_710072a000(reinterpret_cast<UnkResHolder2*>(object), key)) {
            fn_7100143244(reinterpret_cast<UnkTaggedObject*>(object));
            operator delete(object);
            return nullptr;
        }
        set->Insert((u64)object);
        result = object;
    }
    if (out)
        *out = object;
    return result;
}
