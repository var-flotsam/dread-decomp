// Unit 2732 (tools/tu_map.py; no reflected class): a list of objects (count, head, tail, allocator at
// +0x8..+0x20, nodes of value and next), the same shape as src/game_lo/fn_710070f240.cpp. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2732.h"
#include "types.h"

struct UnkListedObject_b52 {
    u8 pad_0x0[0x998];
    u32 field_0x998;
};

struct UnkObjectNode_b52 {
    UnkListedObject_b52* value;
    UnkObjectNode_b52* next;
};

struct UnkObjectList_b52 {
    u64 field_0x0;
    u32 count;
    UnkObjectNode_b52* head;
    UnkObjectNode_b52* tail;
    base::UnkAllocator* allocator;
};

// Removes `object` from the list and frees its node; with `clear`, also clears the object's +0x998
// (even when it was not listed).
void fn_710119ae24(UnkObjectList_b52* self, UnkListedObject_b52* object, bool clear) {
    if (!object)
        return;
    UnkObjectNode_b52* node = self->head;
    for (; node; node = node->next) {
        if (node->value == object)
            break;
    }
    if (node) {
        UnkObjectNode_b52* prev = nullptr;
        UnkObjectNode_b52* cur = self->head;
        UnkObjectNode_b52* next;
        UnkObjectNode_b52* last = nullptr;
        for (;;) {
            if (!cur)
                goto done;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (self->tail == node)
                self->tail = prev;
        } else {
            self->head = next;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        self->allocator->vf_0x48(node);
    }
done:
    if (clear)
        object->field_0x998 = 0;
}
