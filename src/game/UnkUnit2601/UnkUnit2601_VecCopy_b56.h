#pragma once

// Unit 2601 of tools/tu_map.py: growable arrays (base::UnkVector's layout) whose Reserve stays out of
// line (fn_7100f86020, fn_7100f85d24), and their copy constructors (fn_7100f85f00 for 0x18-byte
// {file path id, u64, u32} records, fn_7100f85c28 for 0x50-byte records holding such an array):
// the settings are copied, the array reserved to the other's capacity, and each element appended
// with PushBack's growth. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

template <class T>
struct UnkCopyVec_b56 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n);

    __attribute__((always_inline)) void PushBackCopy(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) T(value);
        ++count;
    }

    __attribute__((always_inline)) void AppendAll(const UnkCopyVec_b56& other) {
        Reserve(other.capacity);
        for (const T *p = other.data, *e = p + other.count; p != e; ++p)
            PushBackCopy(*p);
    }

    __attribute__((always_inline)) void CopyFrom(const UnkCopyVec_b56& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(other.capacity);
        for (const T *p = other.data, *e = p + other.count; p != e; ++p)
            PushBackCopy(*p);
    }
};

struct UnkPathRecord18_b56 {
    base::global::CFilePathStrId id;
    u64 field_0x8;
    u32 field_0x10;
};

using UnkPathRecordVec_b56 = UnkCopyVec_b56<UnkPathRecord18_b56>;
void fn_7100f86020(UnkPathRecordVec_b56* self, u32 n);
template <>
inline void UnkPathRecordVec_b56::Reserve(u32 n) {
    fn_7100f86020(this, n);
}

void fn_7100f85f00(UnkPathRecordVec_b56* self, const UnkPathRecordVec_b56* other);

struct UnkPathRecordsVec_b56 {
    UnkPathRecordVec_b56 vec;
    UnkPathRecordsVec_b56(const UnkPathRecordsVec_b56& other) { fn_7100f85f00(&vec, &other.vec); }
};

struct UnkRecord50b_b56 {
    u32 field_0x0;
    UnkPathRecordsVec_b56 records;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};

using UnkRecord50Vec_b56 = UnkCopyVec_b56<UnkRecord50b_b56>;
void fn_7100f85d24(UnkRecord50Vec_b56* self, u32 n);
template <>
inline void UnkRecord50Vec_b56::Reserve(u32 n) {
    fn_7100f85d24(this, n);
}

void fn_7100f85c28(UnkRecord50Vec_b56* self, const UnkRecord50Vec_b56* other);
// Shrinks the array to n records, destroying the rest (shared with another unit's identical copy).
void fn_7100d90460(UnkRecord50Vec_b56* self, u32 n);
// Copy assignment: truncated to none, then the other's records appended.
UnkRecord50Vec_b56* fn_7100f86254(UnkRecord50Vec_b56* self, const UnkRecord50Vec_b56* other);

struct UnkRecord50VecMember_b56 {
    UnkRecord50Vec_b56 vec;
    UnkRecord50VecMember_b56(const UnkRecord50VecMember_b56& other) { fn_7100f85c28(&vec, &other.vec); }
};
