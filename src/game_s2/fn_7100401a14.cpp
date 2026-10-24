// Starts the central unit's reaction: -1 request id gives 6, otherwise the central unit AI
// component of the owner at 0x18 (when it is one) is handed to fn_7100405854, which gives 5.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent {
    virtual const CClass* vf_0x0();
};
struct UnkOwner {
    u8 unk_0x0[0x240];
    UnkAIComponent* component;
};
struct UnkSelf {
    u8 unk_0x0[0x18];
    UnkOwner* owner;
    u8 unk_0x20[0xc];
    s32 field_0x2c;
};

const CClass* fn_71004030f4();  // CCentralUnitAIComponent::refl_GetClass
void fn_7100405854(UnkAIComponent* component, s32 a1);

u32 fn_7100401a14(UnkSelf* self) {
    if (self->field_0x2c < 0)
        return 6;
    UnkAIComponent* component = self->owner->component;
    UnkAIComponent* found = nullptr;
    if (component && fn_710009d50c(component->vf_0x0(), fn_71004030f4()))
        found = component;
    fn_7100405854(found, self->field_0x2c);
    return 5;
}
