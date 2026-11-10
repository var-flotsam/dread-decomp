#include <new>

#include "base/UnkCloneRecords.h"
#include "types.h"

struct UnkIdVecRecord_b0258n {
    u32 field_0x0;
    base::global::CStrId id;  // 0x8
    base::UnkStrIdVec ids;    // 0x10
};

void fn_7100e07484(UnkIdVecRecord_b0258n* self, const UnkIdVecRecord_b0258n* other) {
    self->field_0x0 = other->field_0x0;
    new (&self->id) base::global::CStrId(other->id);
    new (&self->ids) base::UnkStrIdVec(other->ids);
}
