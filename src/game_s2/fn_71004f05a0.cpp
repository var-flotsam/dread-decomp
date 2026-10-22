// Calls the base routine on the component, then fn_71004f0648 on the member at +0x2a700 with a
// temporary copy of the other's string id (+0x30).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSource {
    u8 pad_0x0[0x30];
    CStrId field_0x30;
};

struct UnkComponent {
    u8 pad_0x0[0x2a700];
    u8 field_0x2a700[1];
};

void fn_7100540238(UnkComponent* self);
void fn_71004f0648(void* member, const CStrId& id);

void fn_71004f05a0(UnkComponent* self, UnkSource* other) {
    fn_7100540238(self);
    fn_71004f0648(self->field_0x2a700, CStrId(other->field_0x30));
}
