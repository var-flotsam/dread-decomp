// Constructor of a query-system parameter named "CRS_MIN_TARGET_DIST": runs the base constructor, sets the vtable
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    // the base constructor initialises the rest
};

extern void* g_unk_7101a395c8[];
void fn_71002c45f8(UnkQueryParam* self);

void fn_710047865c(UnkQueryParam* self) {
    fn_71002c45f8(self);
    self->vtable = &g_unk_7101a395c8[2];
    self->field_0x8 = UnkStrId("CRS_MIN_TARGET_DIST", true);
}
