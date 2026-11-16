// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): makes sure a model is loaded for a file
// path: when the map at +0x29c8 has none and the registry knows the file, creates the model, puts it on layer 0
// and keeps it under the path. False when the file is unknown. Names and layouts are placeholders.
#include <new>

#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::global::CFilePathStrId;

// The model's parts this uses: its scene node at +8 and its data at +0x138.
struct UnkModelParts_e0484c {
    u8 pad_0x0[0x8];
    UnkSceneNode node;
    u8 pad_0x20[0x118];
    UnkModelData* data;
};
static_assert(__builtin_offsetof(UnkModelParts_e0484c, data) == 0x138, "layout");

struct UnkPathHashed_e0484c {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkPathNode_e0484c {
    CFilePathStrId key;
    UnkModel* value;             // 0x8
    UnkPathNode_e0484c* next;    // 0x10
    UnkPathNode_e0484c* prev;    // 0x18
    UnkPathNode_e0484c* chain;   // 0x20

    UnkPathNode_e0484c(const CFilePathStrId& k, UnkModel* v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkPathNode_e0484c*& Next() { return next; }
    UnkPathNode_e0484c*& Prev() { return prev; }
};

// A hash map by file path whose nodes are also kept in a list.
struct UnkPathMap_e0484c {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkPathNode_e0484c** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkPathNode_e0484c* tail;  // 0x38
    UnkPathNode_e0484c* head;  // 0x40

    UnkPathNode_e0484c* Find(const CFilePathStrId& key) {
        UnkPathNode_e0484c** bucket = nullptr;
        if (bucket_count != 0) {
            const UnkPathHashed_e0484c* h = reinterpret_cast<const UnkPathHashed_e0484c*>(key.data);
            u32 i = (h->hash_lo ^ h->hash_hi) % divisor;
            bucket = buckets + i;
        }
        if (!bucket)
            return nullptr;
        for (UnkPathNode_e0484c* n = *bucket; n; n = n->chain)
            if (n->key.data == key.data)
                return n;
        return nullptr;
    }
    void Insert(const CFilePathStrId& key, UnkModel* value) {
        UnkPathNode_e0484c** bucket = nullptr;
        if (bucket_count != 0) {
            const UnkPathHashed_e0484c* h = reinterpret_cast<const UnkPathHashed_e0484c*>(key.data);
            u32 i = (h->hash_lo ^ h->hash_hi) % divisor;
            bucket = buckets + i;
        }
        UnkPathNode_e0484c* old = *bucket;
        auto* node = static_cast<UnkPathNode_e0484c*>(allocator->vf_0x40(sizeof(UnkPathNode_e0484c), 8, flags));
        if (node)
            new (node) UnkPathNode_e0484c(key, value);
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

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton_e0484c(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The game manager's members this uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerModels_e0484c {
    u8 pad_0x0[0x29c8];
    UnkPathMap_e0484c field_0x29c8;
};

bool fn_7100e0484c(UnkGameManager* manager, const CFilePathStrId* path) {
    auto* self = reinterpret_cast<UnkGameManagerModels_e0484c*>(manager);
    if (self->field_0x29c8.Find(*path))
        return true;
    UnkRegistry_0x70* registry = Singleton_e0484c<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0);
    bool known;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, path);
        known = fn_7100087850(registry, &s);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (!known)
        return false;
    UnkRenderManager* render = g_unk_7101d47300;
    UnkModel* created = nullptr;
    fn_71006cae2c(render->field_0x218, *path, &created, &g_unk_7101d470e0);
    fn_7100219ae0(render, created ? &reinterpret_cast<UnkModelParts_e0484c*>(created)->node : nullptr, 0);
    UnkModel* model = created;
    fn_71001c0968(reinterpret_cast<UnkModelParts_e0484c*>(model)->data, true);
    fn_71001436a0(model, 0);
    self->field_0x29c8.Insert(*path, model);
    return true;
}
