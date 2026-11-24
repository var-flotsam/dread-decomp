// Assignment operator of a record of three string ids and 36 bytes of other data: the first id is copied,
// the other two are moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_2e6288 {
    u64 field_0x0;
    CStrId sName;
    CStrId sBone;
    u8 field_0x18[16];
    u8 field_0x28[20];
    u32 pad_0x3c;
    CStrId sEntity;
};

UnkRecord_2e6288* fn_71002e6288(UnkRecord_2e6288* self, UnkRecord_2e6288* other) {
    self->sName = other->sName;
    self->sBone.ReleaseAndTake(other->sBone);
    __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
    __builtin_memcpy(self->field_0x28, other->field_0x28, sizeof(self->field_0x28));
    self->sEntity.ReleaseAndTake(other->sEntity);
    return self;
}
