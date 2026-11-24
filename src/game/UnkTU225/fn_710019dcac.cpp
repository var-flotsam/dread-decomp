// The scene registry's constructor (game/UnkGfxSceneRegistry.h): its map with 0x400 buckets and
// 0x4000 pooled nodes, then the vector with room for 0x1000. A map that already holds entries is
// emptied: its nodes go back to the allocator and the buckets are cleared.
#include <new>

#include "base/UnkBlockFreeList.h"
#include "game/UnkGfxSceneRegistry.h"
#include "game/UnkTU225.h"
#include "types.h"

using base::input::UnkPooledChainedMap;

extern char g_unk_7101a1e2a8[];  // vtable
void fn_710019e58c(UnkPooledChainedMap* map, u32 buckets, u32 nodes);

void fn_710019dcac(UnkGfxSceneRegistry* self) {
    self->vtable = g_unk_7101a1e2a8 + 0x10;
    UnkPooledChainedMap* m = &self->map;
    m->alloc = GetDefaultAllocator();
    m->flags = 0xe;
    m->size = 0;
    m->buckets.data = nullptr;
    m->buckets.count = 0;
    m->buckets.capacity = 0;
    m->buckets.step = 10;
    m->buckets.field_0x14 = true;
    m->buckets.flags = 0xe;
    base::IAllocator* a = GetDefaultAllocator();
    m->buckets.allocator = a;
    m->last = nullptr;
    m->first = nullptr;
    m->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    m->field_0x50 = 0;
    fn_710008377c(&m->pool);
    m->field_0x80 = 0;
    m->field_0x84 = false;
    fn_710019e58c(m, 0x400, 0x4000);
    new (&self->list) base::global::CRntVector<void*>(0xf);
    self->list.Reserve(0x1000);
    if (m->size) {
        auto* node = static_cast<UnkGfxCameraNode*>(m->first);
        while (node) {
            UnkGfxCameraNode* next = node->next;
            m->alloc->vf_0x48(node);
            node = next;
        }
        __builtin_memset(m->buckets.data, 0, m->buckets.capacity * u64(8));
        m->size = 0;
        m->last = nullptr;
        m->first = nullptr;
    }
}
