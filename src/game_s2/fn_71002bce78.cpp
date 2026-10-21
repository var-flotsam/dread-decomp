// Copy assignment of a record with two string ids around a counter and a trailing word (four identical copies).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRec {
    u8 pad_0x0[8];
    CStrId field_0x8;
    u32 field_0x10;
    CStrId field_0x18;
    u64 field_0x20;
};

UnkRec* fn_71002bce78(UnkRec* self, const UnkRec* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
        self->field_0x20 = other->field_0x20;
    }
    return self;
}
