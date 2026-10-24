// Two identical constructors of a class (vtable unk_7101a20c98) with an empty string id at 8, an
// allocator handle at 0x10 and an array of 8-byte values at 0x20 that is resized to 11 zeroed
// entries: fn_710025f17c, fn_7100262c34.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkVector;
using base::global::UnkStringData;

// A string id constructed empty: the pool is created first when there is none yet.
struct UnkEmptyId_710025f17c {
    UnkStringData* data;

    __attribute__((always_inline)) UnkEmptyId_710025f17c() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkAllocatorHandle_710025f17c {
    UnkAllocator* allocator = fn_7100083990();
    u32 flags = 0xe;
    u32 field_0xc = 0;
};

template <class T>
struct UnkResizedVector_710025f17c : UnkVector<T> {
    using UnkVector<T>::UnkVector;

    // Resizes to n entries, zeroing the new ones.
    __attribute__((always_inline)) void Resize(u32 n) {
        if (this->count < n) {
            T* p;
            if (this->capacity >= n) {
                p = this->data;
            } else {
                p = static_cast<T*>(this->allocator->vf_0x40((u64)n * sizeof(T), 8, this->flags));
                if (this->data) {
                    __builtin_memcpy(p, this->data, (u64)this->count * sizeof(T));
                    this->allocator->vf_0x48(this->data);
                }
                this->capacity = n;
                this->data = p;
            }
            __builtin_memset(p + this->count, 0, (u64)(n - this->count) * sizeof(T));
        } else if (this->count == n) {
            return;
        }
        this->count = n;
    }
};

extern void* g_unk_7101a20c98[];

struct UnkObject_710025f17c {
    void** vtable;
    UnkEmptyId_710025f17c field_0x8;
    UnkAllocatorHandle_710025f17c field_0x10;
    UnkResizedVector_710025f17c<u64> field_0x20;
    u64 field_0x48;
    u64 field_0x50;

    __attribute__((always_inline)) UnkObject_710025f17c()
        : vtable(g_unk_7101a20c98 + 2), field_0x20(0xe),
          field_0x48(0), field_0x50(0) {
        field_0x20.Resize(11);
    }
};

void fn_710025f17c(UnkObject_710025f17c* self) { new (self) UnkObject_710025f17c; }

void fn_7100262c34(UnkObject_710025f17c* self) { new (self) UnkObject_710025f17c; }
