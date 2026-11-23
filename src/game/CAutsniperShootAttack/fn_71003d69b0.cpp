// Advances the timer at 0x88 by `dt`; once it reaches the limit at 0xfc of the object at 0x28
// it is cleared and, unless the state at 0x50 is already 5, the autsniper switches to state 5:
// its AI component (when it is one) gets the flag at 0x27248, the component's byte at 0x179d is
// cleared, the counter at 0x9c is reset and the three follow-up steps run.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIComponent_3d69b0 {
    virtual const CClass* vf_0x0();
};
struct UnkAutsniperAI : UnkAIComponent_3d69b0 {
    u8 unk_0x8[0x1795];
    u8 field_0x179d;
};
struct UnkEntity_3d69b0 {
    u8 unk_0x0[0x240];
    UnkAutsniperAI* component;
};
struct UnkLimits_3d69b0 {
    u8 unk_0x0[0xfc];
    f32 limit;
};
struct UnkSelf_3d69b0 {
    u8 unk_0x0[8];
    UnkEntity_3d69b0* entity;
    u8 unk_0x10[0x18];
    UnkLimits_3d69b0* limits;
    u8 unk_0x30[0x20];
    s32 state;
    u8 unk_0x54[0x34];
    f32 timer;
    u8 unk_0x8c[0x10];
    u32 field_0x9c;
};

void fn_71003d7158(UnkSelf_3d69b0* self);
void fn_71003d7250(UnkSelf_3d69b0* self);
void fn_71003d7348(UnkSelf_3d69b0* self);

void fn_71003d69b0(UnkSelf_3d69b0* self, f32 dt) {
    f32 timer = self->timer + dt;
    self->timer = timer;
    if (!(timer >= self->limits->limit))
        return;
    self->timer = 0.0f;
    if (self->state == 5)
        return;
    self->state = 5;
    UnkAIComponent_3d69b0* component = self->entity->component;
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
