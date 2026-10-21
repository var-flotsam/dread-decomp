// Copy constructor of a pair of path-id lists: each list copies its settings, reserves the
// source's capacity and appends a copy of every id.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIdList_710103a240 : base::UnkVector<CFilePathStrId> {
    void Reserve(u32 capacity);  // fn_7100018450

    __attribute__((always_inline)) void Append(const CFilePathStrId& id) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        // Copied bitwise, then the reference taken: count is reloaded after the store.
        CFilePathStrId* slot = &data[count];
        base::global::UnkStringData* value = id.data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++count;
    }

    __attribute__((always_inline)) void CopyFrom(const UnkPathIdList_710103a240& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(other.capacity);
        for (const CFilePathStrId *it = other.data, *end = it + other.count; it != end; ++it)
            Append(*it);
    }
};

struct UnkPathIdListPair_710103a240 {
    UnkPathIdList_710103a240 first;
    UnkPathIdList_710103a240 second;
};

void fn_710103a240(UnkPathIdListPair_710103a240* self, const UnkPathIdListPair_710103a240* other) {
    self->first.CopyFrom(other->first);
    self->second.CopyFrom(other->second);
}
