// Assignment operator of a record of two string ids and 16 bytes of other data (after a first word
// that isn't copied): the first id is copied, the second is moved (the source keeps the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_2e1dc0 {
    u64 field_0x0;
    CStrId sName;
    CStrId sBone;
    u8 field_0x18[16];
};

UnkRecord_2e1dc0* fn_71002e1dc0(UnkRecord_2e1dc0* self, UnkRecord_2e1dc0* other) {
    if (self != other) {
        self->sName = other->sName;
        self->sBone.ReleaseAndTake(other->sBone);
        __builtin_memcpy(self->field_0x18, other->field_0x18, sizeof(self->field_0x18));
    }
    return self;
}
