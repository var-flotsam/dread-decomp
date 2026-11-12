// CPlasmaBeamGun's unit (tools/tu_map.py): filling a pool of up to 31 effect objects (the pool of
// src/game/CFXPool/CDefinition/CDefinition_PathPool_ovb0068.cpp). The object array grows to the wanted
// count (at least one), and each new slot gets an object the render manager's pool creates from the path
// (fn_7100339d48 for switch objects, fn_7100339900 for models), put on layer 0 (fn_7100219ae0) and
// switched off; its bit in the in-use mask is cleared. Names and layouts are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAbilitySonar.h"
#include "game/CFXPool/CDefinition.h"
#include "game/CPlasmaBeamGun.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkFxPooledObject;
struct UnkSonarModel_ovb0085;
// As src/game/CScorpiusFXComponent/CScorpiusFXComponent_Gas_ovb0008.cpp declares it (it returns the node).
void* fn_7100339900(UnkRenderPool* pool, const CFilePathStrId* path, UnkSonarModel_ovb0085** out,
                    const CStrId* name);
// As src/game/CGrappleBeamComponent/CGrappleBeamComponent_Effects_ovb0047.cpp declares it.

struct UnkFxObjectPool_ovb0135 {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkFxPooledObject*> objects;  // 0x10
    u32 used;                                     // 0x38
};

template <class T, class R, R (*Create)(UnkRenderPool*, const CFilePathStrId*, T**, const CStrId*),
          void (*Switch)(UnkSwitchObject*, bool)>
static inline void Fill(UnkFxObjectPool_ovb0135* self, const CFilePathStrId* path, u32 wanted) {
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
            Switch(reinterpret_cast<UnkSwitchObject*>(created), false);
            *fn_7100852548(&self->objects, i) = reinterpret_cast<UnkFxPooledObject*>(created);
            self->used &= ~(1u << i);
        }
    }
}

void fn_71008523c4(UnkFxObjectPool_ovb0135* self, const CFilePathStrId* path, u32 count) {
    Fill<UnkSwitchObject, void*, fn_7100339d48, fn_7100146dac>(self, path, count);
}

void fn_7100852698(UnkFxObjectPool_ovb0135* self, const CFilePathStrId* path, u32 count) {
    Fill<UnkSonarModel_ovb0085, void*, fn_7100339900, fn_7100145e94>(self, path, count);
}

// The effect cache (the model manager, game/UnkModelManager.h) keeps one pool per effect path, in a
// chained hash map at +0x38 keyed by the path (its hash's halves xor-ed, modulo the bucket capacity).
struct UnkFxCache;
struct UnkFxObjectPool_ovb0135Base;
bool fn_71011aa9d0(UnkFxCache* cache);

struct UnkFxPoolNode_ovb0135 {
    CFilePathStrId key;
    UnkFxObjectPool_ovb0135* value;  // 0x8
    UnkFxPoolNode_ovb0135* next;     // 0x10: the list of all nodes
    UnkFxPoolNode_ovb0135* prev;     // 0x18
    UnkFxPoolNode_ovb0135* chain;    // 0x20: the next node in the bucket
};

struct UnkFxPoolMap_ovb0135 {
    using Node = UnkFxPoolNode_ovb0135;
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
    void Insert(const CFilePathStrId& key, UnkFxObjectPool_ovb0135* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = Hash(key) % bucket_capacity;
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

struct UnkFxCache_ovb0135 {
    u8 pad_0x0[0x38];
    UnkFxPoolMap_ovb0135 pools;  // 0x38
};

struct UnkFxPoolVirtual_ovb0135 {
    void** vtable;
    void vf_0x10() { reinterpret_cast<void (*)(UnkFxPoolVirtual_ovb0135*)>(vtable[0x10 / 8])(this); }
};

template <void (*FillPool)(UnkFxObjectPool_ovb0135*, const CFilePathStrId*, u32)>
static inline void Preload(UnkFxCache* cache, const CFilePathStrId* path, u32 count, bool add) {
    if (!fn_71011aa9d0(cache))
        return;
    UnkFxPoolMap_ovb0135* map = &reinterpret_cast<UnkFxCache_ovb0135*>(cache)->pools;
    UnkFxObjectPool_ovb0135* pool;
    bool found;
    if (UnkFxPoolNode_ovb0135* node = map->Find(*path)) {
        pool = node->value;
        found = true;
    } else {
        pool = static_cast<UnkFxObjectPool_ovb0135*>(fn_71000839a0(0x40, 0x1a));
        fn_710104da24(pool);
        found = false;
    }
    reinterpret_cast<UnkFxPoolVirtual_ovb0135*>(pool)->vf_0x10();
    if (add) {
        u32 n = pool->objects.count + count;
        count = n < 0x20 ? n : 0x20;
    }
    FillPool(pool, path, count);
    if (found)
        return;
    map->Insert(*path, pool);
}

void fn_7100851bc0(UnkFxCache* cache, const CFilePathStrId* path, u32 kind, bool flag) {
    Preload<fn_71008523c4>(cache, path, kind, flag);
}

void fn_7100851d6c(UnkFxCache* cache, const CFilePathStrId* path, u32 kind, bool flag) {
    Preload<fn_7100852698>(cache, path, kind, flag);
}

// The plasma beam's effects: registered with the model manager's effect cache once (flag at +0x1870)
// before the base's step (fn_71008548bc), released again before the other base step (fn_7100854db8).
struct UnkPlasmaBeamGun_ovb0135 {
    u8 pad_0x0[0x1870];
    bool fx_registered;  // 0x1870
};

void fn_71008548bc(UnkPlasmaBeamGun_ovb0135* self);
void fn_7100854db8(UnkPlasmaBeamGun_ovb0135* self);

static inline UnkFxCache* FxCache() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return reinterpret_cast<UnkFxCache*>(g_unk_71726b0cd8);
}

template <void (*Preload)(UnkFxCache*, const CFilePathStrId*, u32, bool)>
static inline void PreloadFx(const char* path, u32 count) {
    UnkFxCache* cache = FxCache();
    CFilePathStrId id(path, true);
    Preload(cache, &id, count, false);
}

static inline void ReleaseFx(const char* path) {
    UnkFxCache* cache = FxCache();
    CFilePathStrId id(path, true);
    fn_71011aa5a4(cache, &id);
}

void fn_71008519d8(UnkPlasmaBeamGun_ovb0135* self) {
    if (!self->fx_registered) {
        PreloadFx<fn_7100851bc0>("actors/weapons/plasmabeam/fx/explosion.bcptl", 0xf);
        PreloadFx<fn_7100851bc0>("actors/weapons/plasmabeam/fx/minor_explosion.bcptl", 0xc);
        PreloadFx<fn_7100851d6c>("actors/weapons/plasmabeam/fx/explosion_game.bcmdl", 0xc);
        self->fx_registered = true;
    }
    fn_71008548bc(self);
}

void fn_7100851f18(UnkPlasmaBeamGun_ovb0135* self) {
    if (self->fx_registered) {
        ReleaseFx("actors/weapons/plasmabeam/fx/explosion.bcptl");
        ReleaseFx("actors/weapons/plasmabeam/fx/minor_explosion.bcptl");
        ReleaseFx("actors/weapons/plasmabeam/fx/explosion_game.bcmdl");
        self->fx_registered = false;
    }
    fn_7100854db8(self);
}
