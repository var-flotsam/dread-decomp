// Clears a record: resets its string id to the empty one and frees its array.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    base::UnkVector<u32> field_0x0;
    u8 pad_0x28[0x0];
    CStrId field_0x28;
};

void fn_71003eeb8c(UnkRecord* self) {
    self->field_0x28.Reset();
    self->field_0x0.Free();
}
