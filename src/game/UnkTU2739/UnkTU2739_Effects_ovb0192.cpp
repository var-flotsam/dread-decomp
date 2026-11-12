// Unit 2739 of tools/tu_map.py (0x71011a7f34-0x71011abbd0, no reflected class), the effect cache that
// game/UnkModelManager.h's manager owns: preloading effect pools by kind (the four fill/preload pairs and
// the dispatcher over them), the map of pools' destructor, the creator of the 0x2a0-byte switch object, the
// release of the pending items and pools, and the unit's static initialiser. Names and layouts are
// placeholders.
#include <new>
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkChainedMap.h"
#include "base/UnkDestructors.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkResKey.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXPool/CDefinition.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU132.h"
#include "game/UnkTU2739.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkFxPooledObject;
struct UnkSwitchObject116;
struct UnkTaggedObject116_ovb0168;
struct UnkSwitchModel_ovb0192;

void fn_7100140bbc(UnkTaggedObject116_ovb0168* self, const CStrId* name);
void fn_7100140e4c(UnkSwitchObject116* self, bool enable);
UnkSceneNode* fn_7100a0feac(UnkRenderPool* pool, CFilePathStrId* path, UnkOverlayModel** out, base::global::CStrId* id);
void fn_71011ab1d0(char* self);

using UnkRumbleFileKey_b0192 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The destructor of the map of keyed objects by path: marks the pool, releases every node's key and
// frees the node, then frees the pool's block (as the rumble manager's file map does).
void fn_71011a96d4(char* self) {
    *reinterpret_cast<bool*>(self + 0x84) = true;
    if (*reinterpret_cast<u32*>(self + 0xc)) {
        char* node = *reinterpret_cast<char**>(self + 0x40);
        while (node) {
            char* next = *reinterpret_cast<char**>(node + 0x10);
            reinterpret_cast<UnkRumbleFileKey_b0192*>(node)->~UnkRumbleFileKey_b0192();
            (*reinterpret_cast<base::UnkAllocator**>(self))->vf_0x48(node);
            node = next;
        }
        memset(*reinterpret_cast<void**>(self + 0x10), 0, *reinterpret_cast<u32*>(self + 0x1c) * u64(8));
        *reinterpret_cast<u32*>(self + 0xc) = 0;
        *reinterpret_cast<char**>(self + 0x38) = nullptr;
        *reinterpret_cast<char**>(self + 0x40) = nullptr;
    }
    *reinterpret_cast<char**>(self) = nullptr;
    reinterpret_cast<base::UnkBlockPool*>(self + 0x48)->~UnkBlockPool();
    fn_71011ab1d0(self);
}

struct UnkFxObjectPool_ovb0192 {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkFxPooledObject*> objects;  // 0x10
    u32 used;                                     // 0x38
};

template <class F>
struct SwitchArg;
template <class A, class B>
struct SwitchArg<void (*)(A, B)> {
    using type = A;
};

template <class T, auto Create, auto Switch>
static inline void Fill(UnkFxObjectPool_ovb0192* self, CFilePathStrId* path, u32 wanted) {
    u32 i = self->objects.count;
    u32 n = wanted > 31 ? 31 : (wanted == 0 ? 1 : wanted);
    if (n <= i)
        return;
    self->objects.Reserve(n);
    __builtin_memset(&self->objects.data[self->objects.count], 0, (u64)(n - self->objects.count) * 8);
    self->objects.count = n;
    for (; i < n; ++i) {
        UnkRenderManager* render = g_unk_7101d47300;
        T* object = nullptr;
        Create(render->field_0x218, path, &object, &g_unk_7101d470e0);
        fn_7100219ae0(render, object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(object) + 8) : nullptr,
                      0);
        T* created = object;
        if (created) {
            Switch(reinterpret_cast<typename SwitchArg<decltype(Switch)>::type>(created), false);
            *fn_7100852548(&self->objects, i) = reinterpret_cast<UnkFxPooledObject*>(created);
            self->used &= ~(1u << i);
        }
    }
}

struct UnkResHolder;
bool fn_71011ab410(UnkResHolder* self, const CFilePathStrId& path);

struct UnkModelSetNode_ovb0192 {
    u64 key;
    u64 value;
    UnkModelSetNode_ovb0192* next;
    UnkModelSetNode_ovb0192* prev;
    UnkModelSetNode_ovb0192* chain;

    UnkModelSetNode_ovb0192(u64 k, u64 v) : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkModelSet_ovb0192 {
    base::UnkAllocator* allocator;
    u32 flags;                           // 0x8
    u32 count;                           // 0xc
    UnkModelSetNode_ovb0192** buckets;   // 0x10
    u32 bucket_count;                    // 0x18
    u32 bucket_capacity;                 // 0x1c
    u8 pad_0x20[0x18];
    UnkModelSetNode_ovb0192* tail;  // 0x38
    UnkModelSetNode_ovb0192* head;  // 0x40

    void Insert(u64 key, u64 value) {
        UnkModelSetNode_ovb0192** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            bucket = &buckets[i];
        }
        UnkModelSetNode_ovb0192* old = *bucket;
        auto* node = static_cast<UnkModelSetNode_ovb0192*>(allocator->vf_0x40(sizeof(UnkModelSetNode_ovb0192), 8, flags));
        if (node)
            new (node) UnkModelSetNode_ovb0192(key, value);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            __builtin_memcpy(&node->prev, &tail, sizeof(tail));
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkSwitchModel_ovb0192 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    UnkSceneNode node;
};

// The render pool's creator for a 0x2a0-byte switch object (fn_7100140bbc constructs it with its name),
// loaded from `path` by fn_71011ab410 unless the path is empty (a failed load deletes it), then added to
// the pool's chained hash set keyed by its scene node and returned as that node; `out` gets the object.
__attribute__((noinline)) UnkSceneNode* fn_71011ab2c0(UnkRenderPool* pool, CFilePathStrId* path, UnkSwitchModel_ovb0192** out, CStrId* id) {
    UnkModelSet_ovb0192* set = reinterpret_cast<UnkModelSet_ovb0192*>(pool);
    auto* model = static_cast<UnkSwitchModel_ovb0192*>(fn_71000839a0(0x2a0, 0x38));
    fn_7100140bbc(reinterpret_cast<UnkTaggedObject116_ovb0168*>(model), id);
    UnkSceneNode* result = nullptr;
    if (model) {
        if (path->data != &g_unk_7101d47200->empty &&
            !fn_71011ab410(reinterpret_cast<UnkResHolder*>(model), *path)) {
            model->vf_0x8();
            return nullptr;
        }
        set->Insert((u64)&model->node, (u64)model);
        result = &model->node;
    }
    if (out)
        *out = model;
    return result;
}

__attribute__((noinline)) void fn_71011ab708(UnkFxObjectPool_ovb0192* self, CFilePathStrId* path, u32 count) {
    Fill<UnkOverlayModel, fn_7100a0feac, fn_7100145818>(self, path, count);
}

__attribute__((noinline)) void fn_71011ab88c(UnkFxObjectPool_ovb0192* self, CFilePathStrId* path, u32 count) {
    Fill<UnkSwitchModel_ovb0192, fn_71011ab2c0, fn_7100140e4c>(self, path, count);
}

struct UnkFxObjectPool_ovb0135;
struct UnkFxCache;

struct UnkFxPoolNode_ovb0192 {
    CFilePathStrId key;
    UnkFxObjectPool_ovb0192* value;  // 0x8
    UnkFxPoolNode_ovb0192* next;     // 0x10: the list of all nodes
    UnkFxPoolNode_ovb0192* prev;     // 0x18
    UnkFxPoolNode_ovb0192* chain;    // 0x20: the next node in the bucket
};

struct UnkFxPoolMap_ovb0192 {
    using Node = UnkFxPoolNode_ovb0192;
    base::UnkAllocator* allocator;  // 0x38
    u32 flags;                      // 0x40
    u32 count;                      // 0x44
    Node** buckets;                 // 0x48
    u32 bucket_count;               // 0x50
    u32 bucket_capacity;            // 0x54
    u8 pad_0x58[0x18];
    Node* tail;  // 0x70
    Node* head;  // 0x78

    static u32 Hash(const CFilePathStrId& key) {
        const u32* h = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(key.data) + 0x30);
        return h[0] ^ h[1];
    }
    Node** Slot(const CFilePathStrId& key) {
        if (!bucket_count)
            return nullptr;
        u32 i = Hash(key) % bucket_capacity;
        asm("" : "+r"(i));  // the original computes the index before loading the buckets
        return &buckets[i];
    }
    Node* Find(const CFilePathStrId& key) {
        Node** slot = Slot(key);
        if (!slot)
            return nullptr;
        for (Node* n = *slot; n; n = n->chain)
            if (n->key.data == key.data)
                return n;
        return nullptr;
    }
    void Insert(const CFilePathStrId& key, UnkFxObjectPool_ovb0192* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = Hash(key) % bucket_capacity;
            asm("" : "+r"(i));  // the original computes the index before loading the buckets
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            new (&n->key) CFilePathStrId(key);
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            __builtin_memcpy(&n->prev, &tail, sizeof(Node*));
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkFxCache_ovb0192 {
    u8 pad_0x0[0x38];
    UnkFxPoolMap_ovb0192 pools;  // 0x38
};

struct UnkFxPoolVirtual_ovb0192 {
    void** vtable;
    void vf_0x10() { reinterpret_cast<void (*)(UnkFxPoolVirtual_ovb0192*)>(vtable[0x10 / 8])(this); }
};

template <void (*FillPool)(UnkFxObjectPool_ovb0192*, CFilePathStrId*, u32)>
static inline void Preload(UnkFxCache* cache, const CFilePathStrId* path, u32 count, bool add) {
    UnkFxPoolMap_ovb0192* map = &reinterpret_cast<UnkFxCache_ovb0192*>(cache)->pools;
    UnkFxObjectPool_ovb0192* pool;
    bool found;
    if (UnkFxPoolNode_ovb0192* node = map->Find(*path)) {
        pool = node->value;
        found = true;
    } else {
        pool = static_cast<UnkFxObjectPool_ovb0192*>(fn_71000839a0(0x40, 0x1a));
        fn_710104da24(reinterpret_cast<UnkFxObjectPool_ovb0135*>(pool));
        found = false;
    }
    reinterpret_cast<UnkFxPoolVirtual_ovb0192*>(pool)->vf_0x10();
    if (add) {
        u32 n = pool->objects.count + count;
        count = n < 0x20 ? n : 0x20;
    }
    FillPool(pool, const_cast<CFilePathStrId*>(path), count);
    if (found)
        return;
    map->Insert(*path, pool);
}

void fn_71011aa25c(UnkFxCache* cache, const CFilePathStrId* path, u32 kind, bool flag) {
    Preload<fn_71011ab708>(cache, path, kind, flag);
}

void fn_71011aa400(UnkFxCache* cache, const CFilePathStrId* path, u32 kind, bool flag) {
    Preload<fn_71011ab88c>(cache, path, kind, flag);
}

static inline UnkFxCache* FxCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

// One case per effect kind, each preloading through the manager's cache (each keeps its own copy
// of the lazy creation).
#define PRELOAD_CASE(kind, fn) \
    case kind:                 \
        fn(FxCache(), path, count, flag); \
        break;

// Preloads the effects of a kind (0, 3, 4 or 8) through the effect cache; other kinds do nothing.
void fn_71011aa0d4(void* unused, u32 kind, const CFilePathStrId* path, u32 count, bool flag) {
    switch (kind) {
        PRELOAD_CASE(0, fn_7100851bc0)
        PRELOAD_CASE(3, fn_7100851d6c)
        PRELOAD_CASE(4, fn_71011aa25c)
        PRELOAD_CASE(8, fn_71011aa400)
    }
}

struct UnkFxObject_ovb0041;
void* fn_7100846678(void* self, const void* path, u32 a, u32 b);
void* fn_71010973c0(void* self, const void* path, u32 a, u32 b);
void* fn_71011a9c98(void* self, const void* path, u32 a, u32 b);

void* fn_71011a9c2c(void* self, u32 kind, const void* path, u32 a, u32 b) {
    switch (kind) {
    case 0:
        // the flags arrive unmasked, as the original passes them on
        return reinterpret_cast<void* (*)(UnkModelManager*, const void*, u32, u32)>(fn_71003b7db0)(
            static_cast<UnkModelManager*>(self), path, a, b);
    case 3:
        return fn_7100846678(self, path, a, b);
    case 4:
        return fn_71010973c0(self, path, a, b);
    case 8:
        return fn_71011a9c98(self, path, a, b);
    default:
        return nullptr;
    }
}

struct UnkQueueOwner;

struct UnkPoolValue_a9390 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkPoolNode_a9390 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> key;  // 0x0
    UnkPoolValue_a9390* value;    // 0x8
    UnkPoolNode_a9390* next;      // 0x10
};

struct UnkItemNode_a9390 {
    void* item;
    UnkItemNode_a9390* next;
};

struct UnkManager_a9390 {
    u8 pad_0x0[0x38];
    base::UnkAllocator* allocator;  // 0x38
    u32 flags;                      // 0x40
    u32 count;                      // 0x44
    void** buckets;                 // 0x48
    u32 bucket_count;               // 0x50
    u32 bucket_capacity;            // 0x54
    u8 pad_0x58[0x18];
    UnkPoolNode_a9390* tail;        // 0x70
    UnkPoolNode_a9390* head;        // 0x78
    u8 pad_0x80[0x48];
    u32 item_count;                 // 0xc8
    UnkItemNode_a9390* item_head;   // 0xd0
    UnkItemNode_a9390* item_tail;   // 0xd8
    base::UnkAllocator* item_allocator;  // 0xe0
};
static_assert(__builtin_offsetof(UnkManager_a9390, item_allocator) == 0xe0, "layout");

// Releases the manager's pending items, then its pooled values and the keyed map.
void fn_71011a9390(UnkManager_a9390* self) {
    for (UnkItemNode_a9390* n = self->item_head; n; n = n->next)
        fn_71011a91c4(reinterpret_cast<UnkModelManager*>(self), n->item, 0);
    for (UnkItemNode_a9390* n = self->item_head; n;) {
        UnkItemNode_a9390* next = n->next;
        self->item_allocator->vf_0x48(n);
        n = next;
    }
    self->item_count = 0;
    self->item_head = nullptr;
    self->item_tail = nullptr;
    fn_71011a9980(reinterpret_cast<UnkQueueOwner*>(self));
    for (UnkPoolNode_a9390* n = self->head; n; n = n->next) {
        if (n->value) {
            n->value->vf_0x8();
            n->value = nullptr;
        }
    }
    if (self->count) {
        for (UnkPoolNode_a9390* n = self->head; n;) {
            UnkPoolNode_a9390* next = n->next;
            n->key.~UnkStrIdMemberOf();
            self->allocator->vf_0x48(n);
            n = next;
        }
        memset(self->buckets, 0, self->bucket_capacity * u64(8));
        self->count = 0;
        self->tail = nullptr;
        self->head = nullptr;
    }
}

// The unit's data: the header every initialiser starts with and the path of the model it loads.
struct UnkStatics_71011aba10 {
    UnkTUHeader header;
    CFilePathStrId path;  // 0x18
};

extern UnkStatics_71011aba10 g_unk_7172a0c448 __attribute__((visibility("hidden")));
extern UnkChainedMap g_unk_7172a0c400;
void fn_71011a7f34(void* map);

void fn_71011aba10() {
    InitHeader(g_unk_7172a0c448.header);
    new (&g_unk_7172a0c448.path) CFilePathStrId("system/engine/models/arealightning.bcmdl", true);
    fn_7100000250(fn_7100000c7c, &g_unk_7172a0c448.path, &g_unk_7101a19000);
    new (&g_unk_7172a0c400) UnkChainedMap();
    g_unk_7172a0c400.ResizeBuckets(11);
    fn_7100000250(fn_71011a7f34, &g_unk_7172a0c400, &g_unk_7101a19000);
}
