// Per-frame update: runs fn_710050cd98, takes the owner's object at +0x240 when it is an Emmy AI
// component, stores the tunable fn_7100552e3c picks for it at 0x6b8 and 0x6bc and, when that
// component's object at +0x10 is a character-class Emmy AI component, sets 0x6c8 to
// 2 / (the value at 0x120 of the object its slot 0xd8 returns) * the picked value + 2.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkTunables {
    u8 pad_0x0[0x120];
    f32 field_0x120;
};

struct UnkObject {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkObject*)>(vtable[0])(this); }
    UnkTunables* vf_0xd8() { return reinterpret_cast<UnkTunables* (*)(UnkObject*)>(vtable[27])(this); }
};

struct UnkEmmyAI : UnkObject {
    u8 pad_0x8[8];
    UnkObject* field_0x10;
};

struct UnkOwner {
    u8 pad_0x0[0x240];
    UnkObject* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x6a8];
    f32 field_0x6b8;
    f32 field_0x6bc;
    u8 pad_0x6c0[8];
    f32 field_0x6c8;
};

void fn_710050cd98(UnkSelf* self);
const CClass* fn_710053b934();  // CEmmyAIComponent::refl_GetClass
const CClass* fn_710051e010();  // CCharClassEmmyAIComponent::refl_GetClass
f32 fn_7100552e3c(UnkEmmyAI* ai);

void fn_710050ccc8(UnkSelf* self) {
    fn_710050cd98(self);
    UnkObject* object = self->field_0x8->field_0x240;
    UnkEmmyAI* ai = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710053b934()))
        ai = static_cast<UnkEmmyAI*>(object);
    f32 picked = fn_7100552e3c(ai);
    self->field_0x6b8 = picked;
    self->field_0x6bc = picked;
    if (ai) {
        if (UnkObject* inner = ai->field_0x10) {
            if (fn_710009d50c(inner->vf_0x0(), fn_710051e010())) {
                f32 scale = inner->vf_0xd8()->field_0x120;
                self->field_0x6c8 = 2.0f / scale * self->field_0x6b8 + 2.0f;
            }
        }
    }
}
