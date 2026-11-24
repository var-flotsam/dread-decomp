// Unit 2739: the model manager's destructor. Each of its seven models is released (fn_71011a91c4)
// and cleared, the rest is torn down (fn_71011a9390), then its members are destroyed in reverse:
// two pooled lists, a map and the block at +0x38. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2739.h"
#include "types.h"

extern char g_unk_7101cb56b8[];
extern char g_unk_7101cb56d8[];
extern char g_unk_7101cb56f8[];
extern char g_unk_7101cb5718[];

struct UnkModelManager_l1a8f60;
struct UnkManager_a9390;
void fn_71011a9390(UnkManager_a9390*);
void fn_7100de7894(void* map);
void fn_71011a96d4(char*);

struct UnkListNode_l1a8f60 {
    void* value;
    UnkListNode_l1a8f60* next;
};

// A list whose nodes come from its allocator, with a block pool beside it; the derived and base
// vtables are the list's own.
template <char* Vtable, char* BaseVtable>
struct UnkPooledList_l1a8f60 {
    char* vtable;
    u32 count;
    UnkListNode_l1a8f60* head;  // 0x10
    UnkListNode_l1a8f60* tail;  // 0x18
    base::IAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28
    char* pool_vtable;              // 0x30
    base::IAllocator* pool_allocator;  // 0x38
    UnkBlockFreeList pool;               // 0x40
    u8 pad_0x68[4];
    bool clearing;  // 0x6c

    void FreeNodes() {
        for (UnkListNode_l1a8f60* e = head; e;) {
            UnkListNode_l1a8f60* next = e->next;
            allocator->vf_0x48(e);
            e = next;
        }
    }

    // The destructor: the list's, the pool's, then the base list's.
    void Destroy() {
        vtable = Vtable + 0x10;
        clearing = true;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = nullptr;
        pool_vtable = g_unk_7101a1a4c8 + 0x10;
        if (void* buffer = pool.buffer) {
            fn_71000837f4(&pool);
            pool_allocator->vf_0x48(buffer);
            pool_allocator = nullptr;
        }
        vtable = BaseVtable + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};
static_assert(sizeof(UnkPooledList_l1a8f60<g_unk_7101cb56b8, g_unk_7101cb56d8>) == 0x70, "list layout");

struct UnkModelManager_l1a8f60 {
    void* models[7];
    u8 field_0x38[0x88];
    UnkPooledList_l1a8f60<g_unk_7101cb56b8, g_unk_7101cb56d8> list_a;  // 0xc0
    u8 field_0x130[0x48];
    UnkPooledList_l1a8f60<g_unk_7101cb56f8, g_unk_7101cb5718> list_b;  // 0x178
};
static_assert(__builtin_offsetof(UnkModelManager_l1a8f60, list_b) == 0x178, "layout");

void fn_71011a8f60(UnkModelManager_l1a8f60* self) {
    for (u32 i = 0; i < 7; ++i) {
        fn_71011a91c4(reinterpret_cast<UnkModelManager*>(self), self->models[i], 0);
        self->models[i] = nullptr;
    }
    fn_71011a9390(reinterpret_cast<UnkManager_a9390*>(self));
    self->list_b.Destroy();
    fn_7100de7894(self->field_0x130);
    self->list_a.Destroy();
    fn_71011a96d4(reinterpret_cast<char*>(self->field_0x38));
}
