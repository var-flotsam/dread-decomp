// Advances the timer at 0x88 by `dt`; once it reaches the limit at 0xfc of the object at 0x28
// it is cleared and, unless the state at 0x50 is already 5, the autsniper switches to state 5:
// its AI component (when it is one) gets the flag at 0x27248, the component's byte at 0x179d is
// cleared, the counter at 0x9c is reset and the three follow-up steps run.
#include "types.h"

struct UnkReflType;
struct UnkAIComponent {
    virtual const UnkReflType* vf_0x0();
};
struct UnkAutsniperAI : UnkAIComponent {
    u8 unk_0x8[0x1795];
    u8 field_0x179d;
};
struct UnkEntity {
    u8 unk_0x0[0x240];
    UnkAutsniperAI* component;
};
struct UnkLimits {
    u8 unk_0x0[0xfc];
    f32 limit;
};
struct UnkSelf {
    u8 unk_0x0[8];
    UnkEntity* entity;
    u8 unk_0x10[0x18];
    UnkLimits* limits;
    u8 unk_0x30[0x20];
    s32 state;
    u8 unk_0x54[0x34];
    f32 timer;
    u8 unk_0x8c[0x10];
    u32 field_0x9c;
};

bool fn_710009d50c(const UnkReflType* type, const UnkReflType* base);
const UnkReflType* fn_71003da2e0();  // CAutsniperAIComponent::refl_GetClass
void fn_71003d7158(UnkSelf* self);
void fn_71003d7250(UnkSelf* self);
void fn_71003d7348(UnkSelf* self);

void fn_71003d69b0(UnkSelf* self, f32 dt) {
    f32 timer = self->timer + dt;
    self->timer = timer;
    if (!(timer >= self->limits->limit))
        return;
    self->timer = 0.0f;
    if (self->state == 5)
        return;
    self->state = 5;
    UnkAIComponent* component = self->entity->component;
    UnkAutsniperAI* found = fn_710009d50c(component->vf_0x0(), fn_71003da2e0())
                                ? static_cast<UnkAutsniperAI*>(component)
                                : nullptr;
    // Read through the possibly null result of the cast, as the original does.
    *reinterpret_cast<volatile u8*>(reinterpret_cast<u8*>(found) + 0x27248) = 1;
    self->entity->component->field_0x179d = 0;
    self->field_0x9c = 0;
    fn_71003d7158(self);
    fn_71003d7250(self);
    fn_71003d7348(self);
}
