// Unit 392 (tools/tu_map.py): grows an array of 0x40-byte records (a counted reference, a word,
// an array of words and a count) to `n` elements, moving them over. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU377.h"
#include "types.h"

struct UnkRefVectorHolder_b47;

// A counted reference whose move constructor takes a new reference to the other's object.
struct UnkRecordRef_l2c6684 {
    base::UnkRefCounted* ptr;

    UnkRecordRef_l2c6684(UnkRecordRef_l2c6684&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            base::UnkRefCounted* p = other.ptr;
            if (p)
                ++p->ref_count;
            base::UnkRefCounted* old = ptr;
            if (old && --old->ref_count == 0)
                delete old;
            ptr = p;
        }
    }
};

// An array of words in base::global::CRntVector's layout, copied with its settings.
struct UnkWordArray_l2c6684 {
    u32* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkWordArray_l2c6684(const UnkWordArray_l2c6684& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u32));
        count = other.count;
    }

    void Reserve(u32 n) {
        if (capacity < n) {
            u32* p = static_cast<u32*>(allocator->vf_0x40((u64)n * sizeof(u32), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u32));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
};

struct UnkRecord_l2c6684 {
    UnkRecordRef_l2c6684 ref;
    u64 field_0x8;
    UnkWordArray_l2c6684 items;
    u32 field_0x38;

    UnkRecord_l2c6684(UnkRecord_l2c6684&& other) = default;
    ~UnkRecord_l2c6684() { fn_71002b67e8(reinterpret_cast<UnkRefVectorHolder_b47*>(this)); }
};
static_assert(sizeof(UnkRecord_l2c6684) == 0x40, "layout");

void fn_71002c6684(base::global::CRntVector<UnkRecord_l2c6684>* records, u32 n) {
    if (records->capacity >= n)
        return;
    auto* p = static_cast<UnkRecord_l2c6684*>(
        records->allocator->vf_0x40((u64)n * sizeof(UnkRecord_l2c6684), 8, records->flags));
    if (records->data) {
        UnkRecord_l2c6684* src = records->data;
        UnkRecord_l2c6684* dst = p;
        for (u32 i = 0; i < records->count; ++i, ++dst, ++src) {
            new (dst) UnkRecord_l2c6684(static_cast<UnkRecord_l2c6684&&>(*src));
            src->~UnkRecord_l2c6684();
        }
        records->allocator->vf_0x48(records->data);
    }
    records->data = p;
    records->capacity = n;
}
