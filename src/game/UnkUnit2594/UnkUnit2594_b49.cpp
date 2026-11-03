// Unit 2594 (tools/tu_map.py; no class name known): resets a record: when it holds an object (+0x8), clears
// the word at +0x10, hands the object to the owner at +0x28 (vtable slot 0x48) and clears the word at
// +0x14; then releases the string id at +0x0. Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecordOwner_b49a6 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* object);
};

struct UnkRecord2594_b49a6 {
    CStrId id;
    void* object;  // 0x8
    u32 field_0x10;
    u32 field_0x14;
    u8 pad_0x18[0x10];
    UnkRecordOwner_b49a6* owner;  // 0x28
};

void fn_7100f11dc0(UnkRecord2594_b49a6* self) {
    if (self->object) {
        self->field_0x10 = 0;
        self->owner->vf_0x48(self->object);
        self->field_0x14 = 0;
    }
    self->id.Reset();
}
