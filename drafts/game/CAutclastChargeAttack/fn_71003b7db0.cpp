// CAutclastChargeAttack's unit: gives an effect object for a file path from the model manager. Unless
// it goes on a layer, it takes one from the path's pool (mode 1: the first free one; else another,
// taken off the manager's list), else loads a new one onto the render manager. Names and layouts are
// placeholders.
#include "base/UnkSList.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAbilitySonar.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkFxObject_ovb0041;

struct UnkFxPooledObject {
    void** vtable;

    void vf_0x118() { reinterpret_cast<void (*)(UnkFxPooledObject*)>(vtable[0x118 / 8])(this); }
};

struct UnkFxObjectPool;

UnkFxPooledObject* CFXPool_AcquireFreeObject(UnkFxObjectPool* self);
UnkFxPooledObject* fn_710104dc8c(UnkFxObjectPool* self);

// A node of the manager's first map: a pool per file path.
struct UnkFxPoolMapNode_l3b7db0 {
    const UnkStringData* key;
    UnkFxObjectPool* pool;  // 0x8
    u8 pad_0x10[0x10];
    UnkFxPoolMapNode_l3b7db0* next;  // 0x20
};

struct UnkFxListTag_l3b7db0;
using UnkFxList_l3b7db0 = base::UnkSList<UnkFxListTag_l3b7db0>;
using UnkFxListNode_l3b7db0 = UnkFxList_l3b7db0::Node;

static inline UnkFxPoolMapNode_l3b7db0* FindPool(UnkChainedMap* map, const UnkStringData* key) {
    if (map->buckets.count == 0)
        return nullptr;
    u32 bucket = (key->hash[0] ^ key->hash[1]) % map->buckets.capacity;
    UnkFxPoolMapNode_l3b7db0** slot = reinterpret_cast<UnkFxPoolMapNode_l3b7db0**>(&map->buckets.data[bucket]);
    if (!slot)
        return nullptr;
    for (UnkFxPoolMapNode_l3b7db0* node = *slot; node; node = node->next) {
        if (node->key == key)
            return node;
    }
    return nullptr;
}

static inline UnkFxListNode_l3b7db0* Find(UnkFxList_l3b7db0* list, u64 value) {
    for (UnkFxListNode_l3b7db0* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkFxList_l3b7db0* list, UnkFxListNode_l3b7db0* node) {
    UnkFxListNode_l3b7db0* prev = nullptr;
    UnkFxListNode_l3b7db0* it = list->head;
    while (it) {
        UnkFxListNode_l3b7db0* cur = it;
        UnkFxListNode_l3b7db0* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

// Takes an object from the pool filed under `key`; null when the path has
// no pool or the pool none to give.
static inline UnkFxPooledObject* TakePooled(UnkModelManager* self, const UnkStringData* key, u32 mode) {
    UnkFxPoolMapNode_l3b7db0* node = FindPool(&self->map_a, key);
    if (!node)
        return nullptr;
    UnkFxPooledObject* fx;
    if (mode == 1) {
        fx = CFXPool_AcquireFreeObject(node->pool);
    } else {
        fx = fn_710104dc8c(node->pool);
        UnkFxList_l3b7db0* list = reinterpret_cast<UnkFxList_l3b7db0*>(&self->field_0xc0);
        if (UnkFxListNode_l3b7db0* n = Find(list, reinterpret_cast<u64>(fx)))
            Erase(list, n);
    }
    return fx;
}

// The header declares both flags bool; the original tests all of the layer and compares the mode with 1 and 2.
UnkFxObject_ovb0041* fn_71003b7db0(UnkModelManager* self, const void* path_arg, u32 layer, u32 mode) {
    const CFilePathStrId* path = static_cast<const CFilePathStrId*>(path_arg);
    UnkFxPooledObject* fx = nullptr;
    if (layer == 0 && mode != 0) {
        fx = TakePooled(self, path->data, mode);
        if (fx) {
            fx->vf_0x118();
            return reinterpret_cast<UnkFxObject_ovb0041*>(fx);
        }
    }
    if (mode == 2)
        return reinterpret_cast<UnkFxObject_ovb0041*>(fx);
    UnkRenderManager* render = g_unk_7101d47300;
    UnkSwitchObject* object = nullptr;
    fn_7100339d48(render->field_0x218, path, &object, &g_unk_7101d470e0);
    fn_7100219ae0(render, object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(object) + 8) : nullptr,
                  layer);
    return reinterpret_cast<UnkFxObject_ovb0041*>(object);
}
