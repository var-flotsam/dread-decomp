#pragma once

// A 0x48-byte holder found in the fn_710019e76c-family constructors (fn_7100192994, fn_7100198aac):
// an allocator and its flags, then an array of pointers (base::UnkVector<void*>'s layout, same
// flags) resized to 11 zeroed elements, then two cleared words. Names and layouts are placeholders.

#include "base/UnkVector.h"
#include "types.h"

// base::UnkVector<void*>'s layout with a resize that zero-fills the new elements.
struct UnkPtrArray_b70a1 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Resize(u32 n) {
        if (count < n) {
            if (capacity < n) {
                void** p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            }
            __builtin_memset(&data[count], 0, (u64)(n - count) * sizeof(void*));
            count = n;
        } else if (count != n) {
            count = n;
        }
    }
};

struct UnkPtrArrayHolder_b70a1 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 field_0xc;
    UnkPtrArray_b70a1 array;
    u64 field_0x38;
    u64 field_0x40;

    // `far`: the holder lies past +0xff in its owner, where the original keeps the holder's address
    // in a register and stores the array's unaligned run of fields through it.
    __attribute__((always_inline)) explicit UnkPtrArrayHolder_b70a1(bool far) {
        UnkPtrArrayHolder_b70a1* t = this;
        if (far)
            asm("" : "+r"(t));
        allocator = fn_7100083990();
        flags = 0xe;
        t->field_0xc = 0;
        t->array.data = nullptr;
        t->array.count = 0;
        t->array.capacity = 0;
        t->array.step = 10;
        t->array.field_0x14 = true;
        array.flags = 0xe;
        array.allocator = fn_7100083990();
        field_0x38 = 0;
        field_0x40 = 0;
        array.Resize(11);
    }
};
static_assert(sizeof(UnkPtrArrayHolder_b70a1) == 0x48, "UnkPtrArrayHolder_b70a1 layout");
