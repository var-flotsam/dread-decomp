#pragma once

// The body of a singly linked list whose nodes come from an allocator: the count, the head and
// tail nodes, the allocator and the flags its nodes are allocated with (0x28 bytes). Lists with a
// vtable or a word before it (base/CDeviceQueue.h's UnkDeviceList, base/UnkSList.h) have the same
// body at +8. `Node` is any type with a `next` pointer to its own type. Names are placeholders.
//
// The count is a union member: Clang gives union accesses no type-based alias info, so it is
// reloaded after the node links are stored, as in the original (the behaviour tree's pending
// list, fn_71003aa8e0 and its neighbours). An empty list's first push sets the count to 1.

#include "base/global/CRntVector.h"
#include "types.h"

namespace base {

template <class Node>
struct UnkLinkedList {
    union {
        u32 count;  // 0x0
    };
    Node* head;            // 0x8
    Node* tail;            // 0x10
    UnkAllocator* allocator;  // 0x18
    u32 flags;             // 0x20

    // Frees every node and leaves the list empty.
    void Clear() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }

    void PushBack(Node* node) {
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }

    void PushFront(Node* node) {
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

    // Unlinks `node` and frees it; nothing happens if it is not in the list.
    void Remove(Node* node) {
        Node* prev = nullptr;
        for (Node* cur = head; cur;) {
            Node* next = cur->next;
            if (cur == node) {
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
                return;
            }
            prev = cur;
            cur = next;
        }
    }
};

}  // namespace base
