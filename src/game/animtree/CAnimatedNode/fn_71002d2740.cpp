// Appends an entry to a pair of parallel arrays (keys, values) embedded at +0xf0 of the object at
// +0x18: when the count has reached the capacity, both arrays are reallocated (the capacity grows
// by the step). The new key is the owner itself; the value comes from a global object.
#include "base/UnkPairArrays.h"
#include "game/UnkPairData.h"
#include "game/UnkPairOwner.h"
#include "game/UnkValueSource.h"
#include "game/animtree/CAnimTreeElement.h"

// NON_MATCHING(minor): the copy loop compares the widened index in 64 bits (the original compares 32 bits), and the
// first allocation call sets up its alignment argument one instruction later; behaviour is identical
void fn_71002d2740(UnkPairOwner* self) {
    UnkPairData* d = self->field_0x18;
    u64 value = (*fn_71002dba48(self))->field_0x30;
    base::UnkPairArrays* a = &d->arrays;
    if (a->count == a->capacity) {
        u32 new_capacity = a->step + a->count;
        if (a->count < new_capacity) {
            a->Grow(new_capacity);
            a->capacity = new_capacity;
        }
    }
    u64 key = reinterpret_cast<u64>(self);
    a->values[a->count] = value;
    a->keys[a->count] = key;
    a->count++;
}
