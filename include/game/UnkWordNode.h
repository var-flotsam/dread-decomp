#pragma once

#include "types.h"

// Copy-assignment of a singly linked list of words: the nodes are freed, then a node is
// allocated and appended for each of the other's values.
// The link is a union member (no type-based alias info: the count is reloaded after it is set).
struct UnkWordNode {
    u64 value;
    union {
        UnkWordNode* next;
    };
};
