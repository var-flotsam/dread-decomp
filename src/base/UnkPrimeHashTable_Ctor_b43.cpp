// More copies of fn_71001ee1f0 (src/base/UnkPrimeHashTable_Ctor_b42.cpp), the constructor of a hash table
// whose bucket count is rounded up to a prime, in further units. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

struct UnkPrimeBucket_b43a3;

struct UnkPrimeHashTable_b43a3 {
    base::UnkAllocator* allocator0;
    u32 flags0;
    u32 field_0xc;
    UnkPrimeBucket_b43a3** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x24;
    u32 flags;
    base::UnkAllocator* allocator;
    void* field_0x38;
    void* field_0x40;

    __attribute__((always_inline)) UnkPrimeHashTable_b43a3(u32 n, base::UnkAllocator* alloc, u32 fl) {
        if (alloc) {
            allocator0 = alloc;
            flags0 = fl;
            field_0xc = 0;
            data = nullptr;
            count = 0;
            capacity = 0;
            step = 10;
            field_0x24 = true;
            flags = fl;
        } else {
            allocator0 = fn_7100083990();
            flags0 = fl;
            field_0xc = 0;
            data = nullptr;
            count = 0;
            capacity = 0;
            step = 10;
            field_0x24 = true;
            flags = fl;
            alloc = fn_7100083990();
        }
        allocator = alloc;
        field_0x38 = nullptr;
        field_0x40 = nullptr;
        if (!fn_71000669a0(n))
            n = fn_7100066930(n);
        Resize(n);
    }

    __attribute__((always_inline)) void Resize(u32 n) {
        u32 c = count;
        if (c < n) {
            UnkPrimeBucket_b43a3** p;
            if (capacity < n) {
                p = static_cast<UnkPrimeBucket_b43a3**>(
                    allocator->vf_0x40((u64)n * sizeof(UnkPrimeBucket_b43a3*), 8, flags));
                if (data) {
                    memcpy(p, data, (u64)count * sizeof(UnkPrimeBucket_b43a3*));
                    allocator->vf_0x48(data);
                }
                c = count;
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            memset(p + c, 0, (u64)(n - c) * sizeof(UnkPrimeBucket_b43a3*));
        } else if (c <= n) {
            return;
        }
        count = n;
    }
};

#define UNK_PRIME_HASH_TABLE_B43A3(addr)                                                       \
    void fn_##addr(void* self, u32 n, void* alloc, u32 flags) {                               \
        new (self) UnkPrimeHashTable_b43a3(n, static_cast<base::UnkAllocator*>(alloc), flags); \
    }

UNK_PRIME_HASH_TABLE_B43A3(7100de5794)
UNK_PRIME_HASH_TABLE_B43A3(7100de5e48)
UNK_PRIME_HASH_TABLE_B43A3(71010f8a48)
UNK_PRIME_HASH_TABLE_B43A3(710119c080)
