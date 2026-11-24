// CHyperBeamGun's unit (tools/tu_map.py unit 1305): an effect model for a path from the model manager.
// Unless a layer is given or `mode` is 0, the path's pool (the manager's map at +0x38) gives one: mode 1
// takes a free object (fn_710104dc00), other modes take one (fn_710104dc8c) and drop it from the
// manager's list at +0xc0; it is reset (virtual 0x118). Mode 2 stops there; otherwise a new model is
// created by the render manager's pool and put on the layer. Names and layouts are placeholders.
#include "base/UnkChainedMap.h"
#include "base/UnkSList.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkFxPooledObject {
    void** vtable;

    void vf_0x118() { reinterpret_cast<void (*)(UnkFxPooledObject*)>(vtable[0x118 / 8])(this); }
};

struct UnkFxObjectPool;
UnkFxPooledObject* CFXPool_AcquireFreeObject(UnkFxObjectPool* self);
UnkFxPooledObject* fn_710104dc8c(UnkFxObjectPool* self);

struct UnkSonarModel_ovb0085;
// As src/game/CScorpiusFXComponent/CScorpiusFXComponent_Gas_ovb0008.cpp declares it (it returns the node).
void* fn_7100339900(UnkRenderPool* pool, const CFilePathStrId* path, UnkSonarModel_ovb0085** out,
                    const CStrId* name);

// A node of the manager's map: a path's string and its pool.
struct UnkFxPoolNode_l846678 {
    UnkStringData* key;
    UnkFxObjectPool* pool;  // 0x8
    u8 pad_0x10[0x10];
    UnkFxPoolNode_l846678* next;  // 0x20
};

struct UnkFxTakenTag_l846678;
using UnkFxTakenList_l846678 = base::UnkSList<UnkFxTakenTag_l846678>;

struct UnkFxManager_l846678 {
    u8 pad_0x0[0x38];
    UnkChainedMap pools;  // 0x38
    u8 pad_0x80[0x40];
    UnkFxTakenList_l846678 taken;  // 0xc0
};
static_assert(__builtin_offsetof(UnkFxManager_l846678, taken) == 0xc0, "layout");

static inline UnkFxTakenList_l846678::Node* Find(UnkFxTakenList_l846678* list, u64 value) {
    for (UnkFxTakenList_l846678::Node* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkFxTakenList_l846678* list, UnkFxTakenList_l846678::Node* node) {
    UnkFxTakenList_l846678::Node* prev = nullptr;
    UnkFxTakenList_l846678::Node* it = list->head;
    while (it) {
        UnkFxTakenList_l846678::Node* cur = it;
        UnkFxTakenList_l846678::Node* next = it->next;
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

// A pooled object for the path, if the path has a pool: a free one (mode 1), or one taken out of the
// manager's list.
static inline UnkFxPooledObject* Take(UnkFxManager_l846678* self, const CFilePathStrId* path, u32 mode) {
    if (self->pools.buckets.count == 0)
        return nullptr;
    UnkStringData* key = path->data;
    u32 hash = key->hash[0] ^ key->hash[1];
    UnkMapNode** bucket = &self->pools.buckets.data[hash % self->pools.buckets.capacity];
    if (!bucket)
        return nullptr;
    for (auto* n = reinterpret_cast<UnkFxPoolNode_l846678*>(*bucket); n; n = n->next) {
        if (n->key != key)
            continue;
        if (mode == 1)
            return CFXPool_AcquireFreeObject(n->pool);
        UnkFxPooledObject* object = fn_710104dc8c(n->pool);
        if (UnkFxTakenList_l846678::Node* taken = Find(&self->taken, reinterpret_cast<u64>(object)))
            Erase(&self->taken, taken);
        return object;
    }
    return nullptr;
}

void* fn_7100846678(void* owner, const void* p, u32 layer, u32 mode) {
    UnkFxManager_l846678* self = static_cast<UnkFxManager_l846678*>(owner);
    const CFilePathStrId* path = static_cast<const CFilePathStrId*>(p);
    UnkFxPooledObject* object = nullptr;
    if (layer == 0 && mode != 0)
        object = Take(self, path, mode);
    if (object) {
        object->vf_0x118();
        return object;
    }
    if (mode == 2)
        return object;
    UnkRenderManager* render = g_unk_7101d47300;
    UnkOverlayModel* model = nullptr;
    fn_7100339900(render->field_0x218, path, reinterpret_cast<UnkSonarModel_ovb0085**>(&model),
                  &g_unk_7101d470e0);
    fn_7100219ae0(render, model ? &model->node : nullptr, layer);
    return model;
}
