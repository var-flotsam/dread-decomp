// Unit 2563 (CMinimapData hint): copy constructor of a record of two words and a vector of string ids.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "types.h"

struct UnkIdVecRecord_e50240 {
    u32 field_0x0;
    u32 field_0x4;
    base::UnkStrIdVec ids;  // 0x8
};

void fn_7100e50240(UnkIdVecRecord_e50240* self, const UnkIdVecRecord_e50240* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x4 = other->field_0x4;
    new (&self->ids) base::UnkStrIdVec(other->ids);
}
