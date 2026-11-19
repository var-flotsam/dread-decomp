// The copy constructor of a keyed array (base::reflection's unit, 0xb0-byte values copied with
// fn_71000e6fd8 after fn_7100097e4c reserves room, then the keys copied as plain words):
// fn_71000dc190. Like refl_KeyedArrayCopy (base/UnkCloneCalls.h) without the key references. The
// keys are copied through a word type that may alias the array, so the count and the key
// pointers are reloaded as in the original; it is a struct because a may_alias typedef loses the
// attribute as a template argument. Names are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU85.h"
#include "types.h"

struct UnkAliasWord_71000dc190 {
    u64 value;
};

struct UnkValue_71000dc190 {
    u8 data[0xb0];
};

using UnkKeyedArray_71000dc190 = base::global::CRntSmallDictionary<UnkAliasWord_71000dc190, UnkValue_71000dc190>;

void fn_7100097e4c(UnkKeyedArray_71000dc190* self, u32 count);

void fn_71000dc190(UnkKeyedArray_71000dc190* self, UnkKeyedArray_71000dc190* other) {
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
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
