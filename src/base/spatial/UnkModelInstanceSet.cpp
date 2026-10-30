// From unit 322 of tools/tu_map.py (0x710021c4cc-0x710021d8ac, no reflected class): the render
// manager's pool of scene objects (game/UnkRenderManager.h) creating a 0x180-byte named instance
// (fn_7100147c4c constructs one around the polymorphic node of unit 287 at +0x18) that holds a
// counted resource, kept in a chained hash set of pointers like the registry of
// src/base/spatial/UnkDecalProjector.cpp; and a list of shared (atomically counted) playing
// instances dropped by state. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkPairArrays.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

// A counted reference to a resource, set as base::core::CAssetLink's (src/base/core/CAssetLink.cpp).
static inline void SetCountedRef(UnkCountedResource*& ref, UnkCountedResource* res) {
    if (ref != res) {
        if (res)
            ++res->ref_count;
        UnkCountedResource* old = ref;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ref = res;
    }
}

// The polymorphic node of unit 287 (src/lifted/: fn_710020a18c constructs it): the resource it
// draws at +0x78; vf_0x90 updates it.
struct UnkModelNode {
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

    u8 pad_0x8[0x70];
    UnkCountedResource* resource;  // 0x78
    u8 pad_0x80[0xe8];
};
static_assert(sizeof(UnkModelNode) == 0x168, "UnkModelNode placeholder layout");

struct UnkModelInstance {
    u32 flags;
    u32 field_0x4;
    UnkCountedResource* resource;  // 0x8
    UnkModelInstance* self;        // 0x10
    alignas(8) u8 node[sizeof(UnkModelNode)];  // 0x18: an UnkModelNode (its dynamic type unknown here)

    UnkModelNode* Node() { return reinterpret_cast<UnkModelNode*>(node); }
};
static_assert(sizeof(UnkModelInstance) == 0x180, "UnkModelInstance layout");

// Constructs an instance named `name` (its node built with it).
void fn_7100147c4c(UnkModelInstance* self, const base::global::CStrId* name);
void* fn_710020a43c(UnkModelNode* node);  // the node's owned object (+0x108) prepared

struct UnkInstanceNode {
    u64 key;
    u64 value;
    UnkInstanceNode* next;   // the list of all nodes
    UnkInstanceNode* prev;
    UnkInstanceNode* chain;  // the next node in the bucket

    explicit UnkInstanceNode(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the set's list ends).
    UnkInstanceNode*& Next() { return next; }
    UnkInstanceNode*& Prev() { return prev; }
};

// The render manager's pool of scene objects (game/UnkRenderManager.h's UnkRenderPool) as far as
// this file uses it: a chained hash set of the objects, keyed by the pointer's low 32 bits
// (base/UnkChainedMap.h's layout).
struct UnkInstanceSet {
    base::UnkAllocator* allocator;
    u32 flags;                    // 0x8
    u32 count;                    // 0xc
    UnkInstanceNode** buckets;    // 0x10 (base::global::CRntVector)
    u32 bucket_count;             // 0x18
    u32 bucket_capacity;          // 0x1c
    u8 pad_0x20[0x18];
    UnkInstanceNode* tail;  // 0x38
    UnkInstanceNode* head;  // 0x40

    // Adds a node for `key` at the front of its bucket and at the end of the list.
    void Insert(u64 key) {
        UnkInstanceNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkInstanceNode* old = *bucket;
        auto* node = static_cast<UnkInstanceNode*>(allocator->vf_0x40(sizeof(UnkInstanceNode), 8, flags));
        if (node)
            new (node) UnkInstanceNode(key);
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

// The pool's creator for these instances (like fn_7100149930, fn_71006cae2c and fn_7100a0feac for
// its other objects): an instance named `name` drawing `resource` (also given to its node, which is
// then updated), added to the pool's set and returned, also through `out` when given.
UnkModelInstance* fn_710021d3d4(UnkRenderPool* pool, UnkCountedResource* resource, UnkModelInstance** out,
                                const base::global::CStrId* name) {
    UnkInstanceSet* set = reinterpret_cast<UnkInstanceSet*>(pool);
    auto* instance = static_cast<UnkModelInstance*>(fn_71000839a0(sizeof(UnkModelInstance), 0x38));
    fn_7100147c4c(instance, name);
    UnkModelInstance* result = nullptr;
    if (instance) {
        fn_710020a43c(instance->Node());
        SetCountedRef(instance->resource, resource);
        if (resource) {
            SetCountedRef(instance->Node()->resource, resource);
            instance->Node()->vf_0x90();
        }
        set->Insert((u64)instance);
        result = instance;
    }
    if (out)
        *out = instance;
    return result;
}

// A playing instance shared through counted pointers (atomically, at +8; vf_0x10 destroys it), like
// src/game/UnkSoundIdRegistry.cpp's: its string id at +0x20 and its state at +0x2c.
struct UnkMediaInstance {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    u8 pad_0xc[0x14];
    u64 id;         // 0x20: a CStrId's data
    u8 pad_0x28[0x4];
    s32 state;      // 0x2c
};

// Whether an instance is in `state`.
bool fn_710021eec0(UnkMediaInstance* instance, s32 state);

struct UnkMediaRef {
    UnkMediaInstance* ptr;

    UnkMediaRef() : ptr(nullptr) {}
    UnkMediaRef(const UnkMediaRef& other) : ptr(nullptr) { *this = other; }
    UnkMediaRef& operator=(const UnkMediaRef& other) {
        UnkMediaInstance* p = other.ptr;
        if (p)
            __atomic_fetch_add(&p->ref_count, 1, __ATOMIC_SEQ_CST);
        if (UnkMediaInstance* old = ptr)
            Release(old);
        ptr = p;
        return *this;
    }
    ~UnkMediaRef() {
        if (UnkMediaInstance* p = ptr)
            Release(p);
    }
    static void Release(UnkMediaInstance* p) {
        __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
        if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
            p->vf_0x10();
    }
};

// Erases the pair at `key` (the key's id released); returns the position that follows it.
u64* fn_710021d6ec(base::UnkPairArrays* pairs, u64* key);

// The owner of the instances: the playing ones (a CRntVector at +0x18) and the objects bound to an
// instance's id (keys: the objects, values: the ids).
struct UnkMediaSet {
    u8 pad_0x0[0x18];
    UnkMediaRef* items;  // 0x18
    u32 count;           // 0x20
    u8 pad_0x24[0x1c];
    base::UnkPairArrays bound;  // 0x40

    // The key bound to `id`, or the end of the keys.
    u64* FindBound(const u64& id) {
        u32 i = 0;
        for (; i < bound.count; ++i)
            if (bound.values[i] == id)
                break;
        return &bound.keys[i];
    }
    u64* EndBound() { return &bound.keys[bound.count]; }

    void RemoveAt(u32 i) {
        items[i].~UnkMediaRef();
        for (; i < count - 1; ++i) {
            UnkMediaRef* dst = items + i;
            new (dst) UnkMediaRef(*(dst + 1));
            (items + i + 1)->~UnkMediaRef();
        }
        count = count - 1;
    }
};

// Drops the instances in state 1, last first: the object bound to one's id goes back to the render
// manager's pool and is unbound, then the instance leaves the list.
// NON_MATCHING(minor): the three searches of the bound ids are laid out differently (the original
// keeps an unfolded end comparison on the not-found paths and walks byte offsets) and the shifting
// loop keeps another base register; the same loads, calls, atomics and stores in the same order.
void fn_710021cf18(UnkMediaSet* self) {
    if (!self->count)
        return;
    UnkMediaRef* begin = self->items;
    UnkMediaRef* it = begin + self->count;
    do {
        --it;
        if (!fn_710021eec0(it->ptr, 1))
            continue;
        if (self->FindBound(it->ptr->id) != self->EndBound()) {
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(*self->FindBound(it->ptr->id)), 2);
            u64* key = self->FindBound(it->ptr->id);
            if (key != self->EndBound())
                fn_710021d6ec(&self->bound, key);
        }
        for (u32 i = 0; i < self->count; ++i) {
            if (self->items[i].ptr == it->ptr) {
                self->RemoveAt(i);
                break;
            }
        }
    } while (it != begin);
}
