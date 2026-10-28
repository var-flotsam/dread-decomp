// Constructor of a query-system parameter named "DTGT": runs the base constructor, sets the vtable, initialises its fields
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    u8 pad_0x10[0x10];
    f32 field_0x20;
    f32 field_0x24;
};

extern void* g_unk_7101a212d8[];
void fn_71002b7fb0(UnkQueryParam* self);

void fn_71002bcbbc(UnkQueryParam* self) {
    fn_71002b7fb0(self);
    self->vtable = &g_unk_7101a212d8[2];
    self->field_0x20 = -1.0f;
    self->field_0x24 = -1.0f;
    self->field_0x8 = UnkStrId("DTGT", true);
}
