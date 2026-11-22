// Unit 2732 (tools/tu_map.py): takes an object out of the item list singleton, if it is listed, and
// then runs the string base's teardown on it. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "game/UnkTU2732.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkItemNode_l19ad3c {
    void* value;
    UnkItemNode_l19ad3c* next;
};

struct UnkItemList_l19ad3c : base::UnkVtableList<UnkItemNode_l19ad3c> {
    bool Contains(void* object) const {
        for (UnkItemNode_l19ad3c* node = head; node; node = node->next) {
            if (node->value == object)
                return true;
        }
        return false;
    }

    void Remove(void* object) {
        if (!object)
            return;
        UnkItemNode_l19ad3c* node = head;
        for (; node; node = node->next) {
            if (node->value == object)
                break;
        }
        if (!node)
            return;
        UnkItemNode_l19ad3c* prev = nullptr;
        UnkItemNode_l19ad3c* cur = head;
        UnkItemNode_l19ad3c* next;
        UnkItemNode_l19ad3c* last = nullptr;
        for (;;) {
            if (!cur)
                return;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (tail == node)
                tail = prev;
        } else {
            head = next;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        allocator->vf_0x48(node);
    }
};

void fn_710119ad3c(void* self) {
    UnkItemList_l19ad3c* list = static_cast<UnkItemList_l19ad3c*>(g_unk_7172729910);
    if (list && list->Contains(self))
        list->Remove(self);
    CRntString_Destroy(static_cast<base::global::CRntString*>(self));
}
