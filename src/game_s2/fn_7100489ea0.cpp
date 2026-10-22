// Chozo warrior X ultimate grab: after fn_7100489f3c, with the flag at 0x132 set the owner's
// Chozo warrior X AI component (when the object at 0x240 is one) goes to fn_710048fc80;
// otherwise the object at 0x1d0 (when there is one) gets the tunable duration at 0x2c.
#include "types.h"

struct UnkReflType;
struct UnkAIComponent {
    virtual const UnkReflType* vf_0x0();
};
struct UnkOwner {
    u8 unk_0x0[0x1d0];
    void* field_0x1d0;
    u8 unk_0x1d8[0x68];
    UnkAIComponent* component;
};
struct UnkSelf {
    u8 unk_0x0[8];
    UnkOwner* owner;
    u8 unk_0x10[0x122];
    u8 field_0x132;
};
struct UnkTunable {
    u8 unk_0x0[0x2c];
    f32 field_0x2c;
};

extern UnkTunable g_unk_71726ce7d0;
bool fn_710009d50c(const UnkReflType* type, const UnkReflType* base);
const UnkReflType* fn_710048db44();  // CChozoWarriorXAIComponent::refl_GetClass
void fn_7100489f3c(UnkSelf* self);
void fn_710048fc80(UnkAIComponent* component);
void fn_7101160348(void* object, f32 duration);

void fn_7100489ea0(UnkSelf* self) {
    fn_7100489f3c(self);
    if (self->field_0x132) {
        UnkAIComponent* component = self->owner->component;
        if (component) {
            UnkAIComponent* found =
                fn_710009d50c(component->vf_0x0(), fn_710048db44()) ? component : nullptr;
            fn_710048fc80(found);
        } else {
            fn_710048fc80(nullptr);
        }
    } else if (self->owner->field_0x1d0) {
        fn_7101160348(self->owner->field_0x1d0, g_unk_71726ce7d0.field_0x2c);
    }
}
