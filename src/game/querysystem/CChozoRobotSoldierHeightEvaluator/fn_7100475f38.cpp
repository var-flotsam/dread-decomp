// Constructor of a query-system parameter named "HEGT": runs the base constructor, sets the vtable
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    // the base constructor initialises the rest
};

extern void* g_unk_7101a394d0[];
void fn_71002b7fb0(UnkQueryParam* self);

void fn_7100475f38(UnkQueryParam* self) {
    fn_71002b7fb0(self);
    self->vtable = &g_unk_7101a394d0[2];
    self->field_0x8 = UnkStrId("HEGT", true);
}
