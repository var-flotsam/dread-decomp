// Unit 168: selects the entry at an index of a linked list and applies it, unless it is already selected.
// Names and layouts are placeholders.
#include "types.h"

struct UnkListNode168 {
    void* value;
    UnkListNode168* next;
};

struct UnkList168 {
    u32 field_0x0;
    u32 count;
    u8 pad_0x8[0x8];
    UnkListNode168* head;
};

struct UnkOwner168 {
    u8 pad_0x0[0x70];
    UnkList168* list;
};

struct UnkSelector168 {
    u8 pad_0x0[0x18];
    UnkOwner168* owner;
    void* selected;
    s32 index;
};

void fn_7100151474(UnkSelector168* self, void* value);

void fn_71001515ec(UnkSelector168* self, s32 index) {
    if (self->index == index)
        return;
    UnkList168* list = self->owner->list;
    if (index >= list->count)
        return;
    UnkListNode168* node = list->head->next;
    s32 left = index;
    while (left > 0) {
        if (node == nullptr)
            break;
        node = node->next;
        --left;
    }
    self->selected = node->value;
    fn_7100151474(self, node->value);
    self->index = index;
}
