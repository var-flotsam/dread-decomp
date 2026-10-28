// Assignment operator of a record of two string ids and 16 bytes of other data (after a first word
// that isn't copied): the first id is copied, the second is moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    u8 field_0x18[16];
};

UnkRecord* fn_71002e1dc0(UnkRecord* self, UnkRecord* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        self->field_0x10.ReleaseAndTake(other->field_0x10);
        __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
    }
    return self;
}
