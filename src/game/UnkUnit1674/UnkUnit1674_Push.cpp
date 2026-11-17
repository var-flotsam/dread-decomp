// From unit 1674 of tools/tu_map.py (0x7100b1df48-0x7100b1e70c, no reflected class): fn_7100b1e34c finds or
// creates the list for (a, b) (fn_7100b1e1f4) and pushes a new item {p, value, q} at its front. Names and
// layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkUnit1674Item {
    void* field_0x0;
    u32 field_0x8;
    void* field_0x10;
};

struct UnkUnit1674Node {
    UnkUnit1674Item* item;
    UnkUnit1674Node* next;
};

// A singly linked list with its nodes from `allocator` with `flags` (as base/CDeviceQueue.h's list).
struct UnkUnit1674List : base::UnkVtableList<UnkUnit1674Node> {
    void PushFront(UnkUnit1674Item* const& item) {
        auto* node = static_cast<UnkUnit1674Node*>(allocator->vf_0x40(sizeof(UnkUnit1674Node), 8, flags));
        if (!node)
            return;
        node->item = item;
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            node->next = head;
            head = node;
            ++count;
        }
    }
};

UnkUnit1674List* fn_7100b1e1f4(void* a, void* b, bool create);

UnkUnit1674Item* fn_7100b1e34c(void* a, void* b, void* p, void* q, u32 value) {
    UnkUnit1674List* list = fn_7100b1e1f4(a, b, true);
    auto* item = static_cast<UnkUnit1674Item*>(fn_71000839a0(sizeof(UnkUnit1674Item), 0xc));
    item->field_0x0 = p;
    item->field_0x8 = value;
    item->field_0x10 = q;
    list->PushFront(item);
    return item;
}
