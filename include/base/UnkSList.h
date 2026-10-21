#pragma once

// A singly linked list of 8-byte values (count at +8, head and tail at +0x10/+0x18, nodes of
// {value, next} from an allocator at +0x20 with flags at +0x28): inserting before a node
// (fn_7100240a00 and its identical copies) inlines insert-after, push-front and push-back.
// The count is a union member: the original reloads it after storing node links. Names and
// layout are placeholders inferred from the code.

#include "base/UnkVector.h"
#include "types.h"

namespace base {

template <class Tag>
struct UnkSList {
    struct Node {
        u64 value;
        Node* next;
    };
    u64 field_0x0;
    union {
        u32 count;
    };
    Node* head;
    Node* tail;
    UnkAllocator* allocator;
    u32 flags;

    __attribute__((always_inline)) Node* NewNode(const u64& v) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (n) {
            n->value = v;
            n->next = nullptr;
        }
        return n;
    }

    __attribute__((always_inline)) void PushFront(const u64& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            n->next = head;
            head = n;
            ++count;
        }
    }

    __attribute__((always_inline)) void PushBack(const u64& v) {
        Node* n = NewNode(v);
        if (!n)
            return;
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

    __attribute__((always_inline)) void InsertAfter(Node* prev, const u64& v) {
        if (!prev) {
            PushFront(v);
            return;
        }
        if (count == 0) {
            PushBack(v);
            return;
        }
        Node* n = NewNode(v);
        if (!n)
            return;
        Node* next = prev->next;
        prev->next = n;
        n->next = next;
        if (!next)
            tail = n;
        ++count;
    }

    // Does nothing when `pos` isn't in the list.
    void InsertBefore(const u64& v, Node* pos) {
        Node* prev = nullptr;
        for (Node* it = head; it; prev = it, it = it->next)
            if (it == pos) {
                InsertAfter(prev, v);
                return;
            }
    }
};

}  // namespace base
