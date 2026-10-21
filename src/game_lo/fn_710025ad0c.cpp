// Destructor of a class holding a keyed array of 0x60-byte polymorphic values (string-id
// keys) and two hashed node maps: the body clears all three, then each member's destructor
// clears and frees its storage through its allocator.
#include <string.h>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* unk_7101a20c78[];  // vtable (reached through the GOT)

struct UnkValue_710025ad0c {
    virtual ~UnkValue_710025ad0c();
    u8 pad_0x8[0x58];
};

struct UnkKeyedArray_710025ad0c {
    UnkValue_710025ad0c* values;
    base::UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Clear() {
        if (keys && count)
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkStrIdMemberOf();
        if (values && count)
            for (u64 i = 0; i < count; ++i)
                values[i].~UnkValue_710025ad0c();
        count = 0;
    }

    __attribute__((always_inline)) ~UnkKeyedArray_710025ad0c() {
        Clear();
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkNode_710025ad0c {
    u8 pad_0x0[0x30];
    UnkNode_710025ad0c* next;
};

struct UnkNodeMap_710025ad0c;
void fn_71002486a8(UnkNodeMap_710025ad0c* map, UnkNode_710025ad0c* node);  // removes a node

struct UnkNodeMap_710025ad0c {
    u64 field_0x0;
    u32 field_0x8;
    u32 count;
    UnkNode_710025ad0c** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator;
    UnkNode_710025ad0c* field_0x38;
    UnkNode_710025ad0c* head;
    u64 field_0x48;

    __attribute__((always_inline)) void Clear() {
        if (count) {
            for (UnkNode_710025ad0c* n = head; n;) {
                UnkNode_710025ad0c* next = n->next;
                fn_71002486a8(this, n);
                n = next;
            }
            memset(buckets, 0, (u64)bucket_count * 8);
            count = 0;
            field_0x38 = nullptr;
            head = nullptr;
        }
    }

    __attribute__((always_inline)) ~UnkNodeMap_710025ad0c() {
        Clear();
        if (buckets) {
            field_0x18 = 0;
            allocator->vf_0x48(buckets);
            bucket_count = 0;
        }
    }
};

struct UnkStruct_710025ad0c {
    void** vtable;
    UnkKeyedArray_710025ad0c array;
    UnkNodeMap_710025ad0c map1;
    UnkNodeMap_710025ad0c map2;

    __attribute__((always_inline)) ~UnkStruct_710025ad0c() {
        vtable = &unk_7101a20c78[2];
        array.Clear();
        map1.Clear();
        map2.Clear();
    }
};

void fn_710025ad0c(UnkStruct_710025ad0c* self) {
    self->~UnkStruct_710025ad0c();
}
