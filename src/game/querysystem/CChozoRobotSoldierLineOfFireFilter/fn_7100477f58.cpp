// Constructor of a query-system parameter named "LINE_OF_FIRE": runs the base constructor, sets the vtable
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    // the base constructor initialises the rest
};

extern void* g_unk_7101a39598[];
void fn_71002bedc8(UnkQueryParam* self);

void fn_7100477f58(UnkQueryParam* self) {
    fn_71002bedc8(self);
    self->vtable = &g_unk_7101a39598[2];
    self->field_0x8 = UnkStrId("LINE_OF_FIRE", true);
}
