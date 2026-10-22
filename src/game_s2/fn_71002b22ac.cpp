// Constructor of a named scale entry: copies the string id, stores the value, sets the bounds to one,
// constructs the table at +0x18 (0x800 entries of 4 bytes) and then sets +0xc to the scale (one when it
// is not positive) and +0x10 to its reciprocal.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntry {
    CStrId field_0x0;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[4];
    u8 field_0x18[1];
};

void fn_71002b2330(void* table, u32 count, u32 size);

// NON_MATCHING(minor): the scale select is made with the opposite condition and operand order (fcsel ls
// instead of hi)
void fn_71002b22ac(UnkEntry* self, const CStrId* name, f32 value, f32 scale) {
    new (&self->field_0x0) CStrId(*name);
    self->field_0x8 = value;
    self->field_0xc = 1.0f;
    self->field_0x10 = 1.0f;
    fn_71002b2330(self->field_0x18, 0x800, 4);
    f32 s = !(scale <= 0.0f) ? scale : 1.0f;
    self->field_0xc = s;
    self->field_0x10 = 1.0f / s;
}
