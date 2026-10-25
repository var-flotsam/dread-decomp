// Stores the owner's aim component (when the object at 0x10 is a CCharClassAimComponent) at
// 0x2b0 and copies three of its values (0x60, 0x58, 0x7c) into the owner; returns what
// fn_71010c41bc gave. The component is read without a null check, as in the original.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAimObject {
    virtual const CClass* vf_0x0();
};
struct UnkAimComponent : UnkAimObject {
    u8 unk_0x8[0x50];
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u8 unk_0x64[0x18];
    u32 field_0x7c;
};
struct UnkSelf {
    u8 unk_0x0[0x10];
    UnkAimObject* object;
    u8 unk_0x18[0x298];
    UnkAimComponent* aim;
    u8 unk_0x2b8[0x18];
    u32 field_0x2d0;
    u8 unk_0x2d4[0x2a0];
    u32 field_0x574;
    u32 field_0x578;
};

bool fn_71010c41bc();

bool fn_7100343c18(UnkSelf* self) {
    bool result = fn_71010c41bc();
    UnkAimObject* object = self->object;
    UnkAimComponent* aim = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_7100359cb0()))
        aim = static_cast<UnkAimComponent*>(object);
    self->aim = aim;
    self->field_0x578 = aim->field_0x60;
    self->field_0x574 = aim->field_0x58;
    self->field_0x2d0 = aim->field_0x7c;
    return result;
}
