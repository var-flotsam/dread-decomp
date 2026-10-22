// Assignment operator of a record of three string ids and 36 bytes of other data: the first id is copied,
// the other two are moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    u8 field_0x18[16];
    u8 field_0x28[20];
    u32 pad_0x3c;
    CStrId field_0x40;
};

UnkRecord* fn_71002e6288(UnkRecord* self, UnkRecord* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x10.ReleaseAndTake(other->field_0x10);
    __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
    __builtin_memcpy(self->field_0x28, other->field_0x28, sizeof(self->field_0x28));
    self->field_0x40.ReleaseAndTake(other->field_0x40);
    return self;
}
