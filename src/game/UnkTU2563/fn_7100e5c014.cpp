// Unit 2563 (CMinimapData hint): appends an entry under a string id key to a keyed array (entries at
// +0x0, keys at +0x8, count +0x10, capacity +0x14, step +0x18), growing it by the step when full
// (fn_7100e5ba88), and returns the new last entry. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkMinimapEntry_e5c014 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    CStrId id;  // 0x18
    u16 field_0x20;
    u8 pad_0x22[6];

    UnkMinimapEntry_e5c014(const UnkMinimapEntry_e5c014& o)
        : field_0x0(o.field_0x0), field_0x4(o.field_0x4), field_0x8(o.field_0x8), field_0xc(o.field_0xc),
          field_0x10(o.field_0x10), field_0x14(o.field_0x14), id(o.id), field_0x20(o.field_0x20) {}

};
static_assert(sizeof(UnkMinimapEntry_e5c014) == 0x28, "placeholder layout");

struct UnkMinimapEntries_e5c014 {
    UnkMinimapEntry_e5c014* values;  // 0x0
    CStrId* keys;                    // 0x8
    u32 count;                       // 0x10
    u32 capacity;                    // 0x14
    u32 step;                        // 0x18
};

UnkMinimapEntry_e5c014* fn_7100e5c014(UnkMinimapEntries_e5c014* self, const CStrId* key,
                                      const UnkMinimapEntry_e5c014* entry) {
    if (self->count == self->capacity)
        fn_7100e5ba88(self, self->step + self->count);
    new (&self->keys[self->count]) CStrId(*key);
    new (&self->values[self->count]) UnkMinimapEntry_e5c014(*entry);
    UnkMinimapEntry_e5c014* data = self->values;
    u32 count = self->count + 1;
    data = self->values;
    self->count = count;
    // The last entry (the first when the count wrapped to zero).
    return count ? data + count - 1 : data;
}
