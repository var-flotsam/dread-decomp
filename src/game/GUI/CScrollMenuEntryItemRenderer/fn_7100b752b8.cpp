// The GUI::CScrollMenuEntryItemRenderer unit's destructor of an object with a node list at 0x238, as
// fn_71010cf1e8 (src/game/CFakePhysicsMovement/fn_71010cf260.cpp): restore the vtables, free the
// nodes (0x10 bytes, linked at 8) and run the base destructor fn_71011d8e90.
#include "base/UnkVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkScrollEntryNode {
    u64 field_0x0;
    UnkScrollEntryNode* next;
};

extern void* g_unk_7101a9ebd8[];  // the object's vtable
extern void* g_unk_7101a9ee50[];  // the list's vtable

struct UnkScrollEntryNodeList {
    void** vtable;
    u32 count;
    UnkScrollEntryNode* head;
    UnkScrollEntryNode* tail;
    base::UnkAllocator* allocator;

    ~UnkScrollEntryNodeList() {
        vtable = &g_unk_7101a9ee50[2];
        for (UnkScrollEntryNode* n = head; n;) {
            UnkScrollEntryNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkScrollEntryRenderer {
    void** vtable;
    u8 unk_0x8[0x230];
    UnkScrollEntryNodeList list;  // 0x238
};

void fn_7100b752b8(UnkScrollEntryRenderer* self) {
    self->vtable = &g_unk_7101a9ebd8[2];
    self->list.~UnkScrollEntryNodeList();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));  // the base part's destructor
}
