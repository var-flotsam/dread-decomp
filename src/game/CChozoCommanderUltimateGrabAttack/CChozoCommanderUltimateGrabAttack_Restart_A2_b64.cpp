// CChozoCommanderUltimateGrabAttack (fn_7100443508) and CChozoWarriorXUltimateGrabAttack (fn_710048c5d4):
// fn_71004432b8's reset (src/game/CChozoCommanderUltimateGrabAttack/fn_71004432b8.cpp) followed by
// setting the flag at +0x13a. With the flag at +0x13d set the time is the owner's animation float for a
// fixed name hash, otherwise -1.0; the four floats at +0x140 become (0, -1, -1, -1) and the unit's
// timing function runs with the id of the object at +0x98 (0x7fffffff when there is none). Names and
// layouts are placeholders.
#include "game/CChozoCommanderUltimateGrabAttack.h"
#include "game/CChozoWarriorXUltimateGrabAttack.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// As src/game/CChozoCommanderUltimateGrabAttack/fn_71004432b8.cpp and
// src/game/CChozoWarriorXUltimateGrabAttack/fn_710048c384.cpp declare them.
struct UnkEffect;

struct UnkGrabIdObject_b64a2 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkGrabOwner_b64a2 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

// The Warrior's members from +0x98 on sit `Shift` bytes lower.
template <u32 Shift>
struct UnkGrabAttack_b64a2 {
    u8 pad_0x0[0x8];
    UnkGrabOwner_b64a2* field_0x8;
    u8 pad_0x10[0x88 - Shift];
    UnkGrabIdObject_b64a2* field_0x98;
    u8 pad_0xa0[0x9a];
    bool field_0x13a;
    u8 pad_0x13b[0x2];
    u8 field_0x13d;
    u8 pad_0x13e[2];
    f32 field_0x140[4];
};

template <void (*Timing)(UnkEffect*, u32, f32), u32 Shift>
static inline __attribute__((always_inline)) void Restart(UnkGrabAttack_b64a2<Shift>* self) {
    UnkEffect* effect = reinterpret_cast<UnkEffect*>(self);
    if (self->field_0x13d) {
        f32 time = fn_7101177ea0(self->field_0x8->field_0x220, 0xf4061ff25a2163d6);
        self->field_0x140[0] = 0.0f;
        self->field_0x140[1] = -1.0f;
        self->field_0x140[2] = -1.0f;
        self->field_0x140[3] = -1.0f;
        Timing(effect, self->field_0x98 ? self->field_0x98->field_0x10 : 0x7fffffff, time);
    } else {
        self->field_0x140[0] = 0.0f;
        self->field_0x140[1] = -1.0f;
        self->field_0x140[2] = -1.0f;
        self->field_0x140[3] = -1.0f;
        Timing(effect, self->field_0x98 ? self->field_0x98->field_0x10 : 0x7fffffff, -1.0f);
    }
    self->field_0x13a = true;
}

void fn_7100443508(UnkGrabAttack_b64a2<0>* self) { Restart<fn_710043fd04>(self); }
void fn_710048c5d4(UnkGrabAttack_b64a2<8>* self) { Restart<fn_7100488990>(self); }
