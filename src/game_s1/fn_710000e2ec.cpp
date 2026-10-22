// Copy of fn_7100a954c8 (src/game_s4/fn_710090dbb4.cpp) for another element type, naming its own callees and data (the
// comments are adapted from the original's): fn_710000e2ec. Copy-construction of an array of 0x18-byte elements: the
// settings are taken over, the array reserves the other's capacity (out of line, fn_710000e610) and appends a copy of
// each element (fn_710000e7a4).
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem18 {
    u64 unk_0x0[3];
};

struct UnkElem18Vector;
void fn_710000e610(UnkElem18Vector* v, u32 n);
void fn_710000e7a4(UnkElem18Vector* v, const UnkElem18* value);

struct UnkElem18Vector {
    UnkElem18* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    template <void (*Reserve)(UnkElem18Vector*, u32), void (*PushBack)(UnkElem18Vector*, const UnkElem18*)>
    __attribute__((always_inline)) void CopyConstruct(const UnkElem18Vector& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(this, other.capacity);
        for (const UnkElem18 *p = other.data, *end = p + other.count; p != end; ++p)
            PushBack(this, p);
    }
};

void fn_710000e2ec(UnkElem18Vector* self, const UnkElem18Vector* other) {
    self->CopyConstruct<fn_710000e610, fn_710000e7a4>(*other);
}
