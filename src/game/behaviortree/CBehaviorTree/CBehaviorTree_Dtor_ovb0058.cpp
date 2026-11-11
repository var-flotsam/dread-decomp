// behaviortree::CBehaviorTree's unit (tools/tu_map.py unit 548): the tree's destructor (fn_71003aa538). Names and
// layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CGameObject.h"
#include "types.h"

extern char g_unk_7101a29398[];
extern char g_unk_7101a293f8[];
extern char g_unk_7101a29418[];

struct UnkBehaviorTreeEntry_b0058 {
    char* vtable;
    u32 state;
    void* key;                         // +0x10
    UnkBehaviorTreeEntry_b0058* next;  // +0x18
};

// The list of pending entries (+0x18): a list over a block pool (+0x48) with its own vtable.
struct UnkBehaviorTreeList_b0058 {
    char* vtable;
    u32 count;                          // +0x20
    UnkBehaviorTreeEntry_b0058* head;   // +0x28
    UnkBehaviorTreeEntry_b0058* tail;   // +0x30
    base::UnkAllocator* allocator;      // +0x38
    u32 flags;                          // +0x40
    u8 pad_0x2c[4];
    char* pool_vtable;                  // +0x48
    base::UnkAllocator* pool_allocator; // +0x50
    UnkBlockFreeList pool;              // +0x58
    u8 pad_0x80[4];
    bool clearing;                      // +0x84

    void FreeNodes() {
        for (UnkBehaviorTreeEntry_b0058* e = head; e;) {
            UnkBehaviorTreeEntry_b0058* next = e->next;
            allocator->vf_0x48(e);
            e = next;
        }
    }
};

struct UnkBehaviorTree_b0058 {
    char* vtable;
    u8 pad_0x8[0x10];
    UnkBehaviorTreeList_b0058 pending;  // 0x18
    base::UnkRefCounted* field_0x88;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x90
};
static_assert(__builtin_offsetof(UnkBehaviorTree_b0058, path) == 0x90, "layout");

struct UnkCameraCtrl_ovb0049;

void fn_71003aa538(UnkBehaviorTree_b0058* self) {
    self->vtable = g_unk_7101a29398 + 0x10;
    if (self->field_0x88) {
        delete self->field_0x88;
        self->field_0x88 = nullptr;
    }
    self->path.~UnkStrIdMemberOf();
    UnkBehaviorTreeList_b0058& list = self->pending;
    list.vtable = g_unk_7101a293f8 + 0x10;
    list.clearing = true;
    list.FreeNodes();
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
    list.allocator = nullptr;
    list.pool_vtable = g_unk_7101a1a4c8 + 0x10;
    if (void* buffer = list.pool.buffer) {
        fn_71000837f4(&list.pool);
        list.pool_allocator->vf_0x48(buffer);
        list.pool_allocator = nullptr;
    }
    list.vtable = g_unk_7101a29418 + 0x10;
    list.FreeNodes();
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
