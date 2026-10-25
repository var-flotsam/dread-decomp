#pragma once

// The string pool's chained hash set (UnkStringPool+0x58) and the helpers its functions share
// (src/base/global/). Layouts are placeholders inferred from the code. A node is the key (+0,
// the string's CRC-64), its UnkStringData (+0x8) and the next node in its bucket (+0x50). The
// data keeps the heap copy of the string at +0, the reference count at +0x8, a string buffer
// at +0x10 (pointer, 32-bit length, the key again at +0x20) and the copy flag at +0x40.
// The set: bucket count +0x8, hash modulus +0xc, first non-empty bucket +0x28, first node
// +0x30, node allocator +0x38 with its flags at +0x40, node count +0x44, modification count
// +0x48.

#include "base/global/CRntString.h"
#include "types.h"

namespace base::global {

// The set, a bucket index (-1 when out of range) and the set's modification count.
struct UnkPoolBucketRef {
    char* set;
    u32 bucket;
    u32 version;

    UnkPoolBucketRef(char* set, u32 bucket)
        : set(set), bucket(bucket), version(*reinterpret_cast<u32*>(set + 0x48)) {}
};

// Where a lookup in the set ended: the node found (or null) and the node before it.
struct UnkPoolIter {
    char* node;
    char* prev;
    UnkPoolBucketRef ref;

    UnkPoolIter(char* node, char* prev, char* set, u32 bucket) : node(node), prev(prev), ref(set, bucket) {}
};

}  // namespace base::global

char** fn_7100080824(char* set, u32 bucket);  // the address of a bucket
bool fn_710007f240(char* buffer, const char* str);  // the buffer differs from str
void fn_710007ecb4(char* buffer, const char* str, u32 length, s64 key);  // buffer constructor
// The buffer's destructor is fn_710007ecd4 (base/global/CRntString.h): the buffer is a CRntString.
void fn_710007edac(char* buffer, const char* str, u32 length, s64 key);   // buffer assign
void fn_7100080a00(char* set, base::global::UnkPoolIter* it);             // erase
char* fn_7100084030(void* heap, u32 size);    // allocate; the block's first field is the memory
void fn_7100084144(void* heap, char* block);  // free

extern void* g_unk_7101d58d40;  // the heap the string copies come from (reached through the GOT)
