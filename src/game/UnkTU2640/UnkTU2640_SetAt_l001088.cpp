// Unit 2640: assigns a copy of a group to element `index` of an array of 0x78-byte groups (a 0x50-byte record
// and an array of u64 ids), growing the array to the next multiple of its step and resizing it to index + 1
// first. Names and layouts are placeholders.
#include "base/UnkPartFfca00.h"
#include "base/UnkRecordFfca00.h"
#include "base/global/CRntVector.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkElem78.h"
#include "game/UnkRec78Init_b0212.h"
#include "game/UnkTU2640.h"
#include "types.h"

void fn_7101002358(UnkElemArray<UnkRec78Init_b0212>* self, u32 n);

// An array of u64 ids whose copy assignment grows it to the other's capacity first, then copies the ids over.
struct UnkTU2640IdArray : base::global::CRntVector<u64> {
    UnkTU2640IdArray& operator=(const UnkTU2640IdArray& other) {
        if (this != &other) {
            count = 0;
            u32 n = other.capacity;
            u64* p;
            if (capacity < n) {
                p = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(u64));
            count = other.count;
        }
        return *this;
    }
};

struct UnkTU2640Group {
    UnkRecordFfca00 record;
    UnkTU2640IdArray ids;  // 0x50
};
static_assert(sizeof(UnkTU2640Group) == 0x78, "UnkTU2640Group layout");

void fn_7101001088(UnkElemArray<UnkTU2640Group>* self, const u32* index, const UnkTU2640Group* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i) {
        u32 grown = self->capacity;
        if (grown <= i && self->step != 0)
            grown = (i / self->step + 1) * self->step;
        fn_710100147c(reinterpret_cast<UnkElemArray<UnkElem78>*>(self), grown);
        fn_7101002358(reinterpret_cast<UnkElemArray<UnkRec78Init_b0212>*>(self), i + 1);
    }
    self->data[i] = *value;
}
