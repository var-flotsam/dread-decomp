// Deleting destructor of a GUI object (vtable 0x7101a9ebd8) with a node list at 0x238 (its own
// vtable 0x7101a9ee50; the nodes are freed through the list's allocator), then the base's
// destructor (fn_71011d8e90).
#include "base/UnkVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern void* g_unk_7101a9ebd8[];  // the object's vtable
extern void* g_unk_7101a9ee50[];  // the list's vtable

struct UnkListNode {
    u64 field_0x0;
    UnkListNode* next;
};

struct UnkNodeList {
    void** vtable;
    u32 count;
    UnkListNode* head;
    UnkListNode* tail;
    base::UnkAllocator* allocator;

    ~UnkNodeList() {
        vtable = g_unk_7101a9ee50 + 2;
        for (UnkListNode* node = head; node;) {
            UnkListNode* next = node->next;
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkListScreen {
    void** vtable;
    u8 unk_0x8[0x230];
    UnkNodeList field_0x238;
};

void fn_7100b75394(UnkListScreen* self) {
    self->vtable = g_unk_7101a9ebd8 + 2;
    self->field_0x238.~UnkNodeList();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
    operator delete(self);
}
