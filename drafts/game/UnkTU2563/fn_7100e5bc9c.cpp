// Unit 2563 (CMinimapData hint): copy assignment of the keyed array of 0x28-byte entries (entries at
// +0x0, string id keys at +0x8, count +0x10, capacity +0x14): clears it, makes room for the other's
// count and copies the entries, then the keys. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkMinimapEntry_e5bc9c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    CStrId id;  // 0x18
    u16 field_0x20;
    u8 pad_0x22[6];

    UnkMinimapEntry_e5bc9c(const UnkMinimapEntry_e5bc9c& o) : id(o.id), field_0x20(o.field_0x20) {
        u32* d = &field_0x0;
        const u32* s = &o.field_0x0;
        for (int k = 0; k < 6; ++k)
            d[k] = s[k];
    }

};
static_assert(sizeof(UnkMinimapEntry_e5bc9c) == 0x28, "placeholder layout");

struct UnkMinimapEntries_e5bc9c {
    UnkMinimapEntry_e5bc9c* values;  // 0x0
    CStrId* keys;                    // 0x8
    u32 count;                       // 0x10
    u32 capacity;                    // 0x14
};

void fn_7100e4ee60(UnkMinimapEntries_e5bc9c* self);  // Clear.
void fn_7100e5bdf0(UnkMinimapEntries_e5bc9c* self);  // Shrinks both arrays to the count.

UnkMinimapEntries_e5bc9c* fn_7100e5bc9c(UnkMinimapEntries_e5bc9c* self, const UnkMinimapEntries_e5bc9c* other) {
    if (self != other) {
        fn_7100e4ee60(self);
        if (other->count != self->capacity) {
            fn_7100e5bdf0(self);
            fn_7100e5ba88(self, other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->values[i]) UnkMinimapEntry_e5bc9c(other->values[i]);
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->keys[i]) CStrId(other->keys[i]);
    }
    return self;
}
