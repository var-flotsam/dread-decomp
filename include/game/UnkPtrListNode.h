#pragma once

#include "types.h"

// A list of pointers (vtable g_unk_7101aa9ac0), its nodes from `allocator` with `flags`.
struct UnkPtrListNode {
    void* value;
    UnkPtrListNode* next;
};
