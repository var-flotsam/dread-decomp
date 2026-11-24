#pragma once

// A hash map from string id to string id whose nodes are also kept in a list (unit 2604 of tools/tu_map.py
// erases from it: fn_7100f8ede4). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrIdToIdNode {
    base::global::CStrId key;
    base::global::CStrId value;  // 0x8
    u8 pad_0x10[0x18];
    UnkStrIdToIdNode* prev;   // 0x28
    UnkStrIdToIdNode* next;   // 0x30
    UnkStrIdToIdNode* chain;  // 0x38: the next node of the bucket

    UnkStrIdToIdNode*& Next() { return next; }
    UnkStrIdToIdNode*& Prev() { return prev; }
};
static_assert(__builtin_offsetof(UnkStrIdToIdNode, chain) == 0x38, "UnkStrIdToIdNode layout");

struct UnkStrIdToIdMap {
    base::IAllocator* allocator;
    u32 flags;                   // 0x8
    u32 count;                   // 0xc
    UnkStrIdToIdNode** buckets;  // 0x10
    u32 bucket_count;            // 0x18
    u32 divisor;                 // 0x1c: a key's bucket is its hash modulo this
    u8 pad_0x20[0x18];
    UnkStrIdToIdNode* tail;  // 0x38
    UnkStrIdToIdNode* head;  // 0x40
};
static_assert(__builtin_offsetof(UnkStrIdToIdMap, head) == 0x40, "UnkStrIdToIdMap layout");
