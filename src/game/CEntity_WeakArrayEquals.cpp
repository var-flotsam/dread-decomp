// CEntity unit: equality of two arrays of weak references (data, count): the same count and, element
// by element, the same target (a reference's block holds it at +0x10; an empty reference has none).
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntityRefBlock {
    u8 unk_0x0[0x10];
    void* target;
};

struct UnkEntityRef {
    UnkEntityRefBlock* block;

    void* Get() const { return block != nullptr ? block->target : nullptr; }
};

struct UnkEntityRefArray {
    UnkEntityRef* data;
    u32 count;
};

bool fn_7100d8432c(const UnkEntityRefArray* a, const UnkEntityRefArray* b) {
    if (a->count != b->count)
        return false;
    const UnkEntityRef* pa = a->data;
    const UnkEntityRef* pb = b->data;
    for (u64 n = a->count; n != 0; --n, ++pb, ++pa) {
        void* ta = pa->Get();
        void* tb = pb->Get();
        if (tb != ta)
            return false;
    }
    return true;
}
