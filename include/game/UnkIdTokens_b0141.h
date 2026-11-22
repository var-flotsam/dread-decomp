#pragma once

#include "base/UnkCloneRecords.h"
#include "game/UnkIdToken_b0141.h"

struct UnkIdTokens_b0141 {
    char* vtable;
    u32 count;
    UnkIdToken_b0141* head;
    UnkIdToken_b0141* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    char* pool_vtable;  // 0x30
    base::UnkAllocator* pool_parent;
    UnkBlockFreeList pool_blocks;
    u32 field_0x68;
    bool field_0x6c;
};
