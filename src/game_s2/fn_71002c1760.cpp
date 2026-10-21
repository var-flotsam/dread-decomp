// Constructor of a query-system parameter named "FRUSTUM": runs the base constructor, sets the vtable, initialises its fields
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    u8 pad_0x10[0x4];
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
};

extern void* g_unk_7101a21378[];
void fn_71002bedc8(UnkQueryParam* self);

void fn_71002c1760(UnkQueryParam* self) {
    fn_71002bedc8(self);
    self->vtable = &g_unk_7101a21378[2];
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x8 = UnkStrId("FRUSTUM", true);
}
