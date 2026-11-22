// Constructor of a query-system parameter named "FRUSTUM": runs the base constructor, sets the vtable, initialises its fields
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "game/UnkQueryParam.h"
#include "game/querysystem/CFilter.h"
#include "types.h"

using base::global::UnkStrId;

extern void* g_unk_7101a213a8[];

void fn_71002c2014(UnkQueryParam* self) {
    fn_71002bedc8(self);
    self->vtable = &g_unk_7101a213a8[2];
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x8 = UnkStrId("FRUSTUM", true);
}
