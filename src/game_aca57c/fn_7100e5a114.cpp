// Shrink-to-fit of a container of two parallel arrays (records and their string ids) sharing a
// count and capacity: both are reallocated to the count, the elements moved over (each source
// destroyed) and the old arrays freed.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::UnkStringData;

// A string id whose move leaves the pool's empty string behind.
struct UnkMovedStrId {
    UnkStringData* data;

    UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

// A string id that is copied (taking a reference) and destroyed like base::UnkStrIdMember.
struct UnkTableId {
    UnkStringData* data;

    UnkTableId(const UnkTableId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkTableId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMovedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    UnkMovedArray(UnkMovedArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

// Copied word by word (not as one block).
struct UnkRecordWords {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;

    UnkRecordWords(const UnkRecordWords& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        field_0x18 = other.field_0x18;
        field_0x1c = other.field_0x1c;
        field_0x20 = other.field_0x20;
        field_0x24 = other.field_0x24;
    }
};

struct UnkRecord60 {
    UnkRecordWords words;
    UnkMovedStrId field_0x28;
    UnkMovedStrId field_0x30;
    UnkMovedArray field_0x38;

    UnkRecord60(UnkRecord60&& other) = default;
};

void fn_7100e4a1d0(UnkRecord60* record);  // destructor

struct UnkRecordTable {
    UnkRecord60* records;
    UnkTableId* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;
};

// NON_MATCHING(minor): the id copy loop addresses the source by index instead of a moving pointer
void fn_7100e5a114(UnkRecordTable* self) {
    if (self->count == self->capacity)
        return;
    UnkRecord60* records =
        static_cast<UnkRecord60*>(self->allocator->vf_0x40((u64)self->count * sizeof(UnkRecord60), 8, self->flags));
    UnkTableId* ids = static_cast<UnkTableId*>(self->allocator->vf_0x40((u64)self->count * sizeof(UnkTableId), 8, self->flags));
    if (UnkRecord60* old = self->records) {
        UnkRecord60* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkRecord60(static_cast<UnkRecord60&&>(*old));
            fn_7100e4a1d0(old);
        }
        self->allocator->vf_0x48(self->records);
    }
    if (UnkTableId* old = self->ids) {
        for (u64 i = 0; (u32)i < self->count; ++i, ++old) {
            new (&ids[i]) UnkTableId(*old);
            old->~UnkTableId();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = self->count;
}
