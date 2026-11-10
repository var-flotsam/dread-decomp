// Unit 2543 (tools/tu_map.py): a registry of pooled path nodes, each a counted file-path id with a
// blackboard (0x10) and a behavior tree (0x100) inside, linked through +0x1a0 and freed one by one:
// the deleting destructor (fn_7100db4434), the complete one (fn_7100db4360) and the one of the registry
// that owns its block pool (fn_7100db3918). Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkPathRegistry.h"
#include "types.h"

using namespace base::global;

extern char g_unk_7101aa9468[];
extern char g_unk_7101aa9448[];

struct UnkTree_b0238;
void fn_71003aa538(UnkTree_b0238* self);
void fn_7101046ac0(CBlackboard* self);

struct UnkPathNode_b0238 {
    CFilePathStrId key;
    s32 refs;
    u8 pad_0xc[4];
    u8 board[0xf0];  // 0x10
    u8 tree[0xa0];   // 0x100
    UnkPathNode_b0238* next;  // 0x1a0

    void Destroy() {
        fn_71003aa538(reinterpret_cast<UnkTree_b0238*>(tree));
        fn_7101046ac0(reinterpret_cast<CBlackboard*>(board));
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            UnkStringData* d = key.data;
            if (d && d != &pool->empty) {
                --d->ref_count;
                if (key.data->ref_count == 0) {
                    fn_71000803a4(pool, key.data);
                    pool = g_unk_7101d47200;
                }
            }
            key.data = &pool->empty;
        }
    }
};
static_assert(sizeof(UnkPathNode_b0238) == 0x1a8, "layout");

struct UnkPathRegistry {
    char* vtable;
    u32 count;
    UnkPathNode_b0238* head;
    UnkPathNode_b0238* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u8 pad_0x2c[4];
    char* pool_vtable;
    base::UnkAllocator* pool_allocator;
    UnkBlockFreeList pool;
    u32 field_0x68;
    bool clearing;

    void FreeNodes() {
        for (UnkPathNode_b0238* n = head; n;) {
            UnkPathNode_b0238* next = n->next;
            n->Destroy();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

void fn_7100db4434(UnkPathRegistry* self) {
    self->vtable = g_unk_7101aa9468 + 0x10;
    self->FreeNodes();
    operator delete(self);
}

void fn_7100db4360(UnkPathRegistry* self) {
    self->vtable = g_unk_7101aa9468 + 0x10;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

void fn_7100db3918(UnkPathRegistry* self) {
    self->vtable = g_unk_7101aa9448 + 0x10;
    self->clearing = true;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = nullptr;
    self->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    if (void* buffer = self->pool.buffer) {
        fn_71000837f4(&self->pool);
        self->pool_allocator->vf_0x48(buffer);
        self->pool_allocator = nullptr;
    }
    self->vtable = g_unk_7101aa9468 + 0x10;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
