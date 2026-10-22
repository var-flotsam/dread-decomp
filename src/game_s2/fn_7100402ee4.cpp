// Starts the central unit's reaction: -1 request id gives 6, otherwise the central unit AI
// component of the owner at 0x18 (when it is one) is handed to fn_7100405d58, which gives 5.
#include "types.h"

struct UnkReflType;
struct UnkAIComponent {
    virtual const UnkReflType* vf_0x0();
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
    u32 field_0x30;
};

bool fn_710009d50c(const UnkReflType* type, const UnkReflType* base);
const UnkReflType* fn_71004030f4();  // CCentralUnitAIComponent::refl_GetClass
void fn_7100405d58(UnkAIComponent* component, s32 a1, u32 a2);

u32 fn_7100402ee4(UnkSelf* self) {
    if (self->field_0x2c < 0)
        return 6;
    UnkAIComponent* component = self->owner->component;
    UnkAIComponent* found = nullptr;
    if (component && fn_710009d50c(component->vf_0x0(), fn_71004030f4()))
        found = component;
    fn_7100405d58(found, self->field_0x2c, self->field_0x30);
    return 5;
}
