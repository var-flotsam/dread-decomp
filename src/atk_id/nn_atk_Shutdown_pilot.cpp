// Shuts an object down once: every item in its list (linked through the node at +0x3d8 of each item)
// is stopped (fn_710122bee4), set to state 3 and removed (fn_710122c958); then the two sub-objects and
// the object itself are released with fn_710122ad74. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkListNode {
    UnkListNode* prev;
    UnkListNode* next;
};

struct UnkListItem {
    u8 pad_0x0[0x3d8];
    UnkListNode node;

    static UnkListItem* FromNode(UnkListNode* node) {
        return reinterpret_cast<UnkListItem*>(reinterpret_cast<u8*>(node) - __builtin_offsetof(UnkListItem, node));
    }
};

struct UnkListOwner {
    u8 pad_0x0[0x18];
    UnkListNode items;  // 0x18
    bool active;        // 0x28
    u8 pad_0x29[7];
    void* field_0x30;
    void* field_0x38;
};

void fn_710122bee4(UnkListItem* item);
void fn_710122ad74(void* object);

void fn_710122c8c0(UnkListOwner* self) {
    if (!self->active)
        return;
    for (UnkListNode* node = self->items.next; &self->items != node;) {
        UnkListItem* item = UnkListItem::FromNode(node);
        node = node->next;
        fn_710122bee4(item);
        fn_710122c43c(item, 3);
        fn_710122c958(self, item);
    }
    if (self->field_0x30) {
        fn_710122ad74(self->field_0x30);
        self->field_0x30 = nullptr;
    }
    if (self->field_0x38) {
        fn_710122ad74(self->field_0x38);
        self->field_0x38 = nullptr;
    }
    fn_710122ad74(self);
    self->active = false;
}
