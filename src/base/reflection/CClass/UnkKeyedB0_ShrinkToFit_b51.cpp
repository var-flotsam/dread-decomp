// Unit 72 (base::reflection::CClass): shrinking a keyed array of 0xb0-byte records by 8-byte key
// (the layout of src/base/reflection/UnkReflKeyedB0_b44.cpp) to its count: the records are moved
// (fn_7100097d18) and the old ones destroyed (fn_71000e70c4), the keys copied. Names and layouts
// are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkRecordB0_b44a1;  // the record as src/base/reflection/UnkReflKeyedB0_b44.cpp names it
void fn_71000e70c4(UnkRecordB0_b44a1* self);
// As src/game_s1/fn_71000dbc34.cpp declares it.
void fn_7100097d18(void* self, void* other);

struct UnkRecordB0_b51 {
    u64 words[0xb0 / 8];

    UnkRecordB0_b51(UnkRecordB0_b51&& other) { fn_7100097d18(this, &other); }
    ~UnkRecordB0_b51() { fn_71000e70c4(reinterpret_cast<UnkRecordB0_b44a1*>(this)); }
};

struct UnkKeyedB0Array_b51 {
    UnkRecordB0_b51* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100096b54(UnkKeyedB0Array_b51* self) {
    if (self->count == self->capacity)
        return;
    UnkRecordB0_b51* values = static_cast<UnkRecordB0_b51*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkRecordB0_b51), 8, self->flags));
    u64* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)self->count * sizeof(u64), 8, self->flags));
    if (UnkRecordB0_b51* src = self->values) {
        UnkRecordB0_b51* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkRecordB0_b51(static_cast<UnkRecordB0_b51&&>(*src));
            src->~UnkRecordB0_b51();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (u64* src = self->keys) {
        // Copied bytewise: the original reloads the count after each key.
        u64* dst = keys;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src)
            __builtin_memcpy(dst, src, sizeof(u64));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}
