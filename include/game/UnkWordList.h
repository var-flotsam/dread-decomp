#pragma once

#include "base/UnkVtableList.h"
#include "game/UnkWordNode.h"

struct UnkWordList : base::UnkVtableList<UnkWordNode> {
    void PushBack(const u64& value) {
        auto* node = static_cast<UnkWordNode*>(allocator->vf_0x40(sizeof(UnkWordNode), 8, flags));
        if (node) {
            node->value = value;
            node->next = nullptr;
        }
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->next = nullptr;
        }
        tail = node;
        ++count;
    }

    UnkWordList& operator=(const UnkWordList& other) {
        if (this != &other) {
            Clear();
            for (UnkWordNode* node = other.head; node; node = node->next)
                PushBack(node->value);
        }
        return *this;
    }
};
