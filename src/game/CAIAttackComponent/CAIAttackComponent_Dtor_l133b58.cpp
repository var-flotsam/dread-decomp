// CAIAttackComponent's unit (tools/tu_map.py unit 2705): the destructor of a member object with no vtable of
// its own: its three string ids (+0x100, +0xf8, +0xf0), its string (+0xb8) and its node list (+0x10, over a
// block pool). Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU604.h"
#include "types.h"

struct UnkAttackNode_l133b58 {
    void* value;
    UnkAttackNode_l133b58* next;
};

struct UnkAttackList_l133b58 {
    void** vtable;
    u32 count;                          // +0x18
    UnkAttackNode_l133b58* head;        // +0x20
    UnkAttackNode_l133b58* tail;        // +0x28
    base::IAllocator* allocator;      // +0x30
    u32 flags;                          // +0x38
    u8 pad_0x2c[4];
    char* pool_vtable;                  // +0x40
    base::IAllocator* pool_allocator; // +0x48
    UnkBlockFreeList pool;              // +0x50
    u8 pad_0x78[4];
    bool clearing;                      // +0x7c

    void FreeNodes() {
        for (UnkAttackNode_l133b58* n = head; n;) {
            UnkAttackNode_l133b58* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkAttackData_l133b58 {
    u8 pad_0x0[0x10];
    UnkAttackList_l133b58 list;
    u8 pad_0x80[0xb8 - 0x80];
    base::global::CRntString name;      // +0xb8
    u8 pad_0xe8[8];
    base::UnkStrIdMember id_0xf0;
    base::UnkStrIdMember id_0xf8;
    base::UnkStrIdMember id_0x100;
};
static_assert(__builtin_offsetof(UnkAttackData_l133b58, name) == 0xb8, "layout");
static_assert(__builtin_offsetof(UnkAttackData_l133b58, id_0x100) == 0x100, "layout");

void fn_7101133b58(UnkAttackData_l133b58* self) {
    self->id_0x100.~UnkStrIdMemberOf();
    self->id_0xf8.~UnkStrIdMemberOf();
    self->id_0xf0.~UnkStrIdMemberOf();
    CRntString_Destroy(&self->name);
    UnkAttackList_l133b58& list = self->list;
    list.vtable = g_unk_7101a32660 + 2;
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
    list.vtable = g_unk_7101a32680 + 2;
    list.FreeNodes();
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
}
