#pragma once

// A list of u32 flag values built on the stack and handed to fn_71008724f8 (fn_7100eed9ec,
// fn_7100600c98): a singly linked list with a vtable (count at +8, head and tail at +0x10/+0x18,
// nodes of {value, next} from an allocator at +0x20 with flags at +0x28). Every field is a union
// member: the original reloads the tail, the allocator and the flags after storing node links.
// It is 16-byte aligned: fn_7100600c98 puts it at sp+0x10 with a spill slot below.
// Names and layout are placeholders.

#include "base/UnkVector.h"
#include "types.h"

namespace unk_flag_list {

struct alignas(16) UnkFlagList {
    struct Node {
        u32 value;
        Node* next;
    };

    union {
        u32 count;
    };
    union {
        Node* head;
    };
    union {
        Node* tail;
    };
    union {
        base::UnkAllocator* allocator;
    };
    union {
        u32 flags;
    };

    UnkFlagList() : count(0), head(nullptr), tail(nullptr), allocator(fn_7100083990()), flags(0x13) {}
    virtual void vf_0x0();
    ~UnkFlagList() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }

    void PushBack(const u32& value) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (!n)
            return;
        n->value = value;
        n->next = nullptr;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }
};

// The key fn_71008724f8 files the list under (a hidden global per caller).
struct UnkFlagListKey;

}  // namespace unk_flag_list

void fn_71008724f8(void* target, unk_flag_list::UnkFlagListKey* key, unk_flag_list::UnkFlagList* values,
                   u32 a, u32 b, u32 c, u32 d);
