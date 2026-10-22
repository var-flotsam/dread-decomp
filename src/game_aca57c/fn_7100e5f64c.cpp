// Copy-assignment of containers of two parallel arrays (records with out-of-line copy
// constructors, and string ids) sharing a count and capacity: clears this one, reallocates
// when the capacities differ, then copy-constructs the records and the ids. One container type
// per record size, each with its own clear, free, allocate and record copy functions.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

template <u32 Size>
struct UnkRecordN {
    u8 unk_0x0[Size];
};

template <u32 Size>
struct UnkRecordIdArray {
    UnkRecordN<Size>* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
};

template <u32 Size, void (*Clear)(UnkRecordIdArray<Size>*), void (*Free)(UnkRecordIdArray<Size>*),
          void (*Allocate)(UnkRecordIdArray<Size>*, u32), void (*CopyRecord)(UnkRecordN<Size>*, const UnkRecordN<Size>*)>
__attribute__((always_inline)) static inline UnkRecordIdArray<Size>* Assign(UnkRecordIdArray<Size>* self,
                                                                            const UnkRecordIdArray<Size>& other) {
    if (self == &other)
        return self;
    Clear(self);
    if (other.count != self->capacity) {
        Free(self);
        Allocate(self, other.count);
    }
    self->count = other.count;
    for (u64 i = 0; i < self->count; ++i)
        CopyRecord(&self->records[i], &other.records[i]);
    self->count = other.count;
    for (u64 i = 0; i < self->count; ++i) {
        // A bitwise copy (the original reloads the count after it), then the reference.
        base::global::UnkStringData* data = other.ids[i].data;
        __builtin_memcpy(&self->ids[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    return self;
}

#define ASSIGN(addr, size, clear, free, allocate, copy)                                               \
    void clear(UnkRecordIdArray<size>* self);                                                         \
    void free(UnkRecordIdArray<size>* self);                                                          \
    void allocate(UnkRecordIdArray<size>* self, u32 capacity);                                        \
    void copy(UnkRecordN<size>* dst, const UnkRecordN<size>* src);                                    \
    UnkRecordIdArray<size>* fn_##addr(UnkRecordIdArray<size>* self, const UnkRecordIdArray<size>& other) { \
        return Assign<size, clear, free, allocate, copy>(self, other);                                \
    }

ASSIGN(7100e5f64c, 0xa8, fn_7100e4ed10, fn_7100e5f748, fn_7100e5f348, fn_7100e509a4)
ASSIGN(7100e610e0, 0x80, fn_7100e4eb8c, fn_7100e611dc, fn_7100e60df4, fn_7100e5190c)
ASSIGN(7100eb1570, 0x28, fn_7100eb1824, fn_7100eb166c, fn_7100b48944, fn_7100eafa60)
