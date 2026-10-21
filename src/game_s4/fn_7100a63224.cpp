// Move constructor of a record of two string ids, plain fields and an array: the moved-from ids
// become the empty string and the moved-from array gives up its buffer.
#include "base/global/CStrId.h"
#include "base/UnkVector.h"

using base::global::UnkStringData;

struct UnkMovedId {
    UnkStringData* data;

    UnkMovedId(UnkMovedId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkArray(UnkArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkRecord_0x60 {
    UnkMovedId field_0x0;
    u32 field_0x8;
    UnkMovedId field_0x10;
    void* field_0x18;
    void* field_0x20;
    u32 field_0x28;
    UnkArray field_0x30;
    bool field_0x58;

    UnkRecord_0x60(UnkRecord_0x60&& other) = default;
};

void fn_7100a63224(UnkRecord_0x60* self, UnkRecord_0x60* other) {
    new (self) UnkRecord_0x60(static_cast<UnkRecord_0x60&&>(*other));
}
