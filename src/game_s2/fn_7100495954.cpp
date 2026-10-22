// Takes the base's value (fn_7100362648) and, when the owner's object (+0x8, at +0x240) is a
// CChozoWarriorAIComponent whose class data (+0x10) is a CCharClassChozoWarriorAIComponent with
// its flag at +0x7f8 set, returns it only if the byte at +0x14999 of the component is set (else
// 0).
#include "types.h"

struct UnkObject {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkObject*)>(vtable[0])(this); }
};

struct UnkClassData : UnkObject {
    u8 pad_0x8[0x7f0];
    u8 field_0x7f8;
};

struct UnkComponent : UnkObject {
    u8 pad_0x8[8];
    UnkClassData* field_0x10;
    u8 pad_0x18[0x14981];
    u8 field_0x14999;
};

struct UnkOwner {
    u8 pad_0x0[0x240];
    UnkComponent* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[0x8];
    UnkOwner* field_0x8;
};

f32 fn_7100362648(UnkSelf* self);
void* fn_7100497f2c();  // CChozoWarriorAIComponent::refl_GetClass
void* fn_7100496fbc();  // CCharClassChozoWarriorAIComponent::refl_GetClass
bool fn_710009d50c(const void* from, void* to);

f32 fn_7100495954(UnkSelf* self) {
    f32 value = fn_7100362648(self);
    UnkComponent* component = self->field_0x8->field_0x240;
    if (component && fn_710009d50c(component->vf_0x0(), fn_7100497f2c())) {
        UnkClassData* data = component->field_0x10;
        if (data && fn_710009d50c(data->vf_0x0(), fn_7100496fbc()) && data->field_0x7f8) {
            if (!component->field_0x14999)
                value = 0.0f;
        }
    }
    return value;
}
