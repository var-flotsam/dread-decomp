// Constructor of a query-system parameter named "IN_MELEE_PATH": runs the base constructor, sets the vtable
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "game/UnkQueryParam.h"
#include "game/querysystem/CFilter.h"
#include "types.h"

using base::global::UnkStrId;

extern void* g_unk_7101a39538[];

void fn_7100477118(UnkQueryParam* self) {
    CFilter_Construct(self);
    self->vtable = &g_unk_7101a39538[2];
    self->field_0x8 = UnkStrId("IN_MELEE_PATH", true);
}
