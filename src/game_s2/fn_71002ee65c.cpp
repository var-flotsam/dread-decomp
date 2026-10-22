// Clears a record: destroys its owned object (through its vtable slot at +0x10) and resets its string id
// to the empty one.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    u8 field_0x10[32];
    UnkOwned* field_0x30;
};

void fn_71002ee65c(UnkRecord* self) {
    UnkOwned* old = self->field_0x30;
    self->field_0x30 = nullptr;
    if (old)
        old->vf_0x10();
    self->field_0x8.Reset();
}
