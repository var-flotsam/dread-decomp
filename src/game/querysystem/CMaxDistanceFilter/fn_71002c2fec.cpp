// Constructor of a query-system parameter named "MAX_DIST": runs the base constructor, sets the vtable, initialises its fields
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    u8 pad_0x10[0x4];
    u32 field_0x14;
};

extern void* g_unk_7101a21408[];
void fn_71002bedc8(UnkQueryParam* self);

void fn_71002c2fec(UnkQueryParam* self) {
    fn_71002bedc8(self);
    self->vtable = &g_unk_7101a21408[2];
    self->field_0x14 = 0;
    self->field_0x8 = UnkStrId("MAX_DIST", true);
}
