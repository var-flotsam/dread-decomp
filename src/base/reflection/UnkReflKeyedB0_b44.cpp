// A reflected keyed array of 0xb0-byte records by 8-byte key (values at +0, keys at +8, count at
// +0x10, allocator at +0x20): its clear (CCollectionType +0x110), destructor (+0x40), remove by key
// (+0x128), Begin (+0x160) and End (+0x168). The records are destroyed by fn_71000e70c4 and
// move-constructed by fn_7100097d18. Names and layouts are placeholders.
#include <new>

#include "UnkTempIterValue_b44.h"
#include "base/UnkVector.h"
#include "game/UnkTU85.h"
#include "types.h"

struct UnkRecordB0_b44a1;
// As src/game_s1/fn_71000dbc34.cpp declares it.
void fn_7100097d18(void* self, void* other);

struct UnkRecordB0_b44a1 {
    u64 words[0xb0 / 8];

    UnkRecordB0_b44a1(UnkRecordB0_b44a1&& other) { fn_7100097d18(this, &other); }
    ~UnkRecordB0_b44a1() { fn_71000e70c4(this); }
};

struct UnkKeyedB0Array_b44a1 {
    UnkRecordB0_b44a1* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkRecordB0_b44a1* End() { return &values[count]; }
    UnkRecordB0_b44a1* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

__attribute__((always_inline)) static inline void Clear(UnkKeyedB0Array_b44a1* self) {
    if (self->values)
        for (u32 i = 0; i < self->count; ++i)
            self->values[i].~UnkRecordB0_b44a1();
    self->count = 0;
}

void fn_71000dbe9c(UnkKeyedB0Array_b44a1* self) { Clear(self); }

void fn_71000dc294(UnkKeyedB0Array_b44a1* self) {
    Clear(self);
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}

using UnkKeyedB0Iter_b44a1 = UnkTempIterValue<UnkRecordB0_b44a1, u64>;

void fn_71000dc374(UnkKeyedB0Array_b44a1* self, UnkKeyedB0Iter_b44a1* out) {
    *out = UnkKeyedB0Iter_b44a1(self->keys, self->values);
}
void fn_71000dc3d8(UnkKeyedB0Array_b44a1* self, UnkKeyedB0Iter_b44a1* out) {
    *out = UnkKeyedB0Iter_b44a1(self->keys + self->count, self->values + self->count);
}

// The found record is destroyed; the last record is moved into its place (and destroyed), the last
// key copied over, and the count drops.
void fn_71000dbf8c(UnkKeyedB0Array_b44a1* self, const u64* key) {
    UnkRecordB0_b44a1* p = self->Find(*key);
    if (p == self->End())
        return;
    u64 i = p - self->values;
    self->values[(u32)i].~UnkRecordB0_b44a1();
    u32 last = self->count - 1;
    if (last != (u32)i) {
        new (&self->values[(u32)i]) UnkRecordB0_b44a1(static_cast<UnkRecordB0_b44a1&&>(self->values[last]));
        self->values[self->count - 1].~UnkRecordB0_b44a1();
        __builtin_memcpy(&self->keys[(u32)i], &self->keys[self->count - 1], sizeof(u64));
        last = self->count - 1;
    }
    self->count = last;
}
