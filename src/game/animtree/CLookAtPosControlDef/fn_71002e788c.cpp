// Assignment operator of a record of two string ids and 16 and 20 bytes of other data (after a first word
// that isn't copied): the first id is copied, the second is moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_2e788c {
    u64 field_0x0;
    CStrId sName;
    CStrId sBone;
    u8 field_0x18[16];
    u8 field_0x28[20];
};

UnkRecord_2e788c* fn_71002e788c(UnkRecord_2e788c* self, UnkRecord_2e788c* other) {
    if (self != other) {
        self->sName = other->sName;
        self->sBone.ReleaseAndTake(other->sBone);
        __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
        __builtin_memcpy(self->field_0x28, other->field_0x28, sizeof(self->field_0x28));
    }
    return self;
}
