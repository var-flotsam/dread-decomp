// Constructor of a query-system parameter named "MIN_DIST": runs the base constructor, sets the vtable, initialises its fields
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "game/UnkQueryParam.h"
#include "game/querysystem/CFilter.h"
#include "types.h"

using base::global::UnkStrId;

extern void* g_unk_7101a21468[];

void fn_71002c3f3c(UnkQueryParam* self) {
    CFilter_Construct(self);
    self->vtable = &g_unk_7101a21468[2];
    self->field_0x14 = 0;
    self->field_0x8 = UnkStrId("MIN_DIST", true);
}
