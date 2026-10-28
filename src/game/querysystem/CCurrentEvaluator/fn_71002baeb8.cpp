// Copy assignment of a record with two string ids around a counter (six identical copies).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRec {
    u8 pad_0x0[8];
    CStrId field_0x8;
    u32 field_0x10;
    CStrId field_0x18;
};

UnkRec* fn_71002baeb8(UnkRec* self, const UnkRec* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
    }
    return self;
}
