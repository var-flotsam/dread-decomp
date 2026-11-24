#pragma once

#include "base/UnkVtableList.h"
#include "game/UnkList_71011199ccNode.h"
#include "game/UnkTU604.h"

struct UnkList_71011199cc : base::UnkVtableList<UnkList_71011199ccNode> {
    using Node = UnkList_71011199ccNode;

    UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = GetDefaultAllocator();
        flags = 0x13;
    }
    ~UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        base::IAllocator* a = allocator;
        while (n) {
            Node* next = n->next;
            a->vf_0x48(n);
            n = next;
        }
    }
};
