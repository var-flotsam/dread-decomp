// Unit 2640: the move helper of the iterator value over a reflected collection of (string id, record) pairs:
// the ids become the empty string and the record's array gives up its buffer. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkTU2640MovedId {
    UnkStringData* data;

    UnkTU2640MovedId(UnkTU2640MovedId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkTU2640Array {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkTU2640Array(UnkTU2640Array&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkTU2640Record {
    UnkTU2640MovedId id;
    UnkTU2640Array array;  // 0x8

    UnkTU2640Record(UnkTU2640Record&& other) = default;
};

void fn_7101002be8(void* dst, const void* src) {
    base::refl_IterValueMove<UnkTU2640Record, UnkTU2640MovedId>(dst, src);
}
