// Unit 2201 (tools/tu_map.py, no name hint): constructor of a record holding a string id (copied),
// two words, a pointer, a float and a flag. Names and layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdRecord_b48a3 {
    CStrId id;
    s32 field_0x8;
    u32 field_0xc;
    void* field_0x10;
    f32 field_0x18;
    bool field_0x1c;
};

void fn_7100c93bfc(UnkStrIdRecord_b48a3* self, s32 a, const CStrId& id, bool flag, const u32& b, void* p, f32 f) {
    new (&self->id) CStrId(id);
    self->field_0x8 = a;
    self->field_0xc = b;
    self->field_0x10 = p;
    self->field_0x18 = f;
    self->field_0x1c = flag;
}
