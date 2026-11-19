// Unit 83 (base::reflection): appends a record to an owner's two parallel arrays (the records and one
// string id per record), growing them by their step when full, and flags the record when the editor
// default is set. Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;

struct UnkIdRecord18B {
    CStrId id;
    u64 field_0x8;
    u32 field_0x10;
    u16 field_0x14;
};

using UnkIdRecord18BArrays = base::global::CRntSmallDictionary<CStrId, UnkIdRecord18B>;

void fn_71000a6ca8(UnkIdRecord18BArrays* self, u32 new_capacity);

struct UnkRecordOwner83 {
    u8 pad_0x0[0xc0];
    UnkIdRecord18BArrays arrays;  // 0xc0
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71000a5174(UnkRecordOwner83* self, UnkIdRecord18B* record) {
    if (g_unk_7101d5bdd8)
        *reinterpret_cast<u8*>(&record->field_0x14) = 1;
    u32 n = self->arrays.count;
    if (n == self->arrays.capacity)
        fn_71000a6ca8(&self->arrays, self->arrays.step + n);
    new (&self->arrays.keys[self->arrays.count]) CStrId(record->id);
    UnkIdRecord18B* dst = &self->arrays.values[self->arrays.count];
    new (&dst->id) CStrId(record->id);
    dst->field_0x8 = record->field_0x8;
    dst->field_0x10 = record->field_0x10;
    dst->field_0x14 = record->field_0x14;
    ++self->arrays.count;
}
