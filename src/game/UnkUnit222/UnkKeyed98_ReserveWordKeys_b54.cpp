// Unit 222: the reserve of the keyed array of 0x98-byte records by 8-byte key whose destructor is
// in src/game/UnkUnit222/UnkUnit222_b45.cpp; a copy of fn_7100097e4c
// (src/base/reflection/CClass/UnkKeyedB0_ReserveWordKeys_b53.cpp): each record moved
// (fn_7100194ba4) and the old one destroyed (fn_710019506c), the keys copied through the aliasing
// word type. Names are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU212.h"
#include "types.h"

typedef u64 __attribute__((may_alias)) UnkAliasWord_b54a1;

struct UnkRecord98_b54a1 {
    u8 data[0x98];
};

struct UnkKeyedArray98_b54a1 {
    UnkRecord98_b54a1* values;
    UnkAliasWord_b54a1* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkObject_710019506c;
// Moves the record `other` into `self`.
void fn_7100194ba4(UnkRecord98_b54a1* self, UnkRecord98_b54a1* other);

void fn_710019b7e8(UnkKeyedArray98_b54a1* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkRecord98_b54a1* values = static_cast<UnkRecord98_b54a1*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord98_b54a1), 8, self->flags));
        UnkAliasWord_b54a1* keys = static_cast<UnkAliasWord_b54a1*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(u64), 8, self->flags));
        if (self->values) {
            UnkRecord98_b54a1* src = self->values;
            UnkRecord98_b54a1* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_7100194ba4(dst, src);
                fn_710019506c(reinterpret_cast<UnkObject_710019506c*>(src));
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkAliasWord_b54a1* old = self->keys) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j)
                keys[j] = old[j];
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
