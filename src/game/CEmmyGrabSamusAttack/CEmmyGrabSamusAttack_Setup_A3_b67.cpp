// CEmmyGrabSamusAttack's unit (tools/tu_map.py): resets the grab timers (+0x6b4 to +0x6c0), runs the
// setup helper fn_71005068c8 with three fixed distances, then, when the owner's AI component is a
// CEmmyAIComponent whose value (fn_7100552ee4) is positive, seeds +0x6b8/+0x6bc from its random tunable
// (fn_7100552d94) and +0x6c8 from both. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU811.h"
#include "types.h"

// As src/game/UnkStateMachineSetA3_b61.cpp declares it.
struct UnkEmmyAI_b61;
// As src/game_s2/fn_7100552d94.cpp defines it.
struct UnkSelf;

struct UnkEmmyGrabAttack_A3_b67;

struct UnkEmmyGrabAttack_A3_b67 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x6a4];
    f32 field_0x6b4;
    f32 field_0x6b8;
    f32 field_0x6bc;
    f32 field_0x6c0;
    u8 pad_0x6c4[4];
    f32 field_0x6c8;
};

void fn_710050c5b4(UnkEmmyGrabAttack_A3_b67* self) {
    self->field_0x6b4 = 0.0f;
    self->field_0x6b8 = -1.0f;
    self->field_0x6bc = -1.0f;
    self->field_0x6c0 = -1.0f;
    fn_71005068c8(self, 50.0f, 350.0f, 250.0f, true, true);
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    auto* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<UnkEmmyAI_b61*>(ai) : nullptr;
    f32 v = fn_7100552ee4(emmy);
    if (v > 0.0f) {
        f32 r = fn_7100552d94(reinterpret_cast<UnkSelf*>(emmy));
        self->field_0x6b8 = r;
        self->field_0x6bc = r;
        self->field_0x6c8 = (2.0f / v) * r + 2.0f;
    } else {
        self->field_0x6b8 = 0.0f;
        self->field_0x6bc = 0.0f;
    }
}
