// The copy constructor of a keyed array (base::reflection's unit, 0xb0-byte values copied with
// fn_71000e6fd8 after fn_7100097e4c reserves room, then the keys copied as plain words):
// fn_71000dc190. Like refl_KeyedArrayCopy (base/UnkCloneCalls.h) without the key references. The
// keys are copied through a word type that may alias the array, so the count and the key
// pointers are reloaded as in the original. Names are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU85.h"
#include "types.h"

typedef u64 __attribute__((may_alias)) UnkAliasWord_71000dc190;

struct UnkValue_71000dc190 {
    u8 data[0xb0];
};

struct UnkKeyedArray_71000dc190 {
    UnkValue_71000dc190* values;
    UnkAliasWord_71000dc190* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100097e4c(UnkKeyedArray_71000dc190* self, u32 count);

void fn_71000dc190(UnkKeyedArray_71000dc190* self, UnkKeyedArray_71000dc190* other) {
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->field_0x18 = other->field_0x18;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100097e4c(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        fn_71000e6fd8(&self->values[i], &other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        self->keys[i] = other->keys[i];
}
