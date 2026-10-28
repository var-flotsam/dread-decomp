// Copy constructors of records holding a vector of 8-byte values: a word, a file path, a word
// and the vector, or a string id and the vector (base::UnkVector's layout, which takes the
// source's settings, reserves its capacity and copies the values with memcpy).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkWordVector {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            u64* p = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }

    __attribute__((always_inline)) UnkWordVector(const UnkWordVector& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u64));
        count = other.count;
    }
};

struct UnkPathWords {
    u32 field_0x0;
    CFilePathStrId path;
    u64 field_0x10;
    UnkWordVector values;
};

void fn_7100b7fbb0(UnkPathWords* self, const UnkPathWords* other) { new (self) UnkPathWords(*other); }
void fn_7100b7fcc0(UnkPathWords* self, const UnkPathWords* other) { new (self) UnkPathWords(*other); }

// A string id and a vector of 8-byte values.
struct UnkIdWords {
    base::global::CStrId id;
    UnkWordVector values;
};

void fn_7100b7837c(UnkIdWords* self, const UnkIdWords* other) { new (self) UnkIdWords(*other); }
void fn_7100b7847c(UnkIdWords* self, const UnkIdWords* other) { new (self) UnkIdWords(*other); }
