// When the owner faces the target (the sign of its direction's x equals the sign of the x offset to
// the target) and the target's object at +0x1b8 fails a test, hands the owner's component at +0x240
// to fn_71004554d8 if its class is the commander AI component.
#include "types.h"

struct UnkComponent {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkDirection {
    u8 pad_0x0[8];
    f32 field_0x8;
};

struct UnkBody {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x170];
    void* field_0x1b8;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    f32 field_0x44;
    u8 pad_0x48[0x1f8];
    UnkComponent* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
};

UnkBody* fn_7100d7c990(UnkOwner* object);
UnkDirection* fn_710002614c(void* object);
bool fn_7101161088(void* object);
void* fn_710044e708();  // CChozoCommanderAIComponent::refl_GetClass
bool fn_710009d50c(const void* from, void* to);
void fn_71004554d8(UnkComponent* component);

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

void fn_7100428010(UnkSelf* self) {
    UnkBody* target = fn_7100d7c990(self->field_0x8);
    if (target) {
        f32 facing = Sign(fn_710002614c(self->field_0x8->field_0x38)->field_0x8);
        f32 side = Sign(target->field_0x44 - self->field_0x8->field_0x44);
        if (facing == side && target->field_0x1b8 && !fn_7101161088(target->field_0x1b8)) {
            UnkComponent* component = self->field_0x8->field_0x240;
            if (component && fn_710009d50c(component->vf_0x0(), fn_710044e708()))
                fn_71004554d8(component);
        }
    }
}
