// Appends an entry to a pair of parallel arrays (keys, values) embedded at +0xf0 of the object at
// +0x18: when the count has reached the capacity, both arrays are reallocated (the capacity grows
// by the step). The new key is the owner itself; the value comes from a global object.
#include "base/UnkPairArrays.h"

struct UnkValueSource {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkPairData {
    u8 pad_0x0[0xf0];
    base::UnkPairArrays arrays;
};

struct UnkPairOwner {
    u8 pad_0x0[0x18];
    UnkPairData* field_0x18;
};

UnkValueSource** fn_71002dba48();

// NON_MATCHING(minor): the copy loop compares the widened index in 64 bits (the original compares 32 bits), and the
// first allocation call sets up its alignment argument one instruction later; behaviour is identical
void fn_71002d2740(UnkPairOwner* self) {
    UnkPairData* d = self->field_0x18;
    u64 value = (*fn_71002dba48())->field_0x30;
    base::UnkPairArrays* a = &d->arrays;
    if (a->count == a->capacity) {
        u32 new_capacity = a->step + a->count;
        if (a->count < new_capacity) {
            a->Grow(new_capacity);
            a->capacity = new_capacity;
        }
    }
    u64 key = reinterpret_cast<u64>(self);
    __builtin_memcpy(&a->values[a->count], &value, sizeof(u64));
    __builtin_memcpy(&a->keys[a->count], &key, sizeof(u64));
    a->count++;
}
