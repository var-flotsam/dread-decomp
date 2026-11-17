// CNailongAIComponent's unit: after the base step (fn_7101106490) the timer at +0x14840 restarts and a new
// delay (+0x148c0) is picked at random between the two bounds of the owner's (+0x30) Nailong AI component's
// tunables: those at +0x146e8 for the attack at +0x146c8, those at +0x14780 for any other. Names and layouts
// are placeholders.
#include <stdlib.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkShakernautAI_b64a4;
void fn_7101106490(UnkShakernautAI_b64a4* self, void* which);

// CNailongAIComponent's class (what fn_710061f464 returns).
extern __attribute__((visibility("hidden"))) CClass g_unk_71726f7e68;

// A tunable object (reflected: its vtable first).
struct UnkNailongDelays_l6218f4 {
    void** vtable;
    u8 pad_0x8[0xe4];
    f32 field_0xec;
    f32 field_0xf0;
    f32 field_0xf4;
};

struct UnkNailongAI_l6218f4 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x146e0];
    UnkNailongDelays_l6218f4* field_0x146e8;
    u8 pad_0x146f0[0x90];
    UnkNailongDelays_l6218f4* field_0x14780;
};
static_assert(__builtin_offsetof(UnkNailongAI_l6218f4, field_0x14780) == 0x14780, "AI layout");

struct UnkNailongOwner_l6218f4 {
    u8 pad_0x0[0x240];
    UnkNailongAI_l6218f4* ai;  // 0x240
};

struct UnkNailongAIComponent_l6218f4 {
    u8 pad_0x0[0x30];
    UnkNailongOwner_l6218f4* owner;  // 0x30
    u8 pad_0x38[0x14690];
    u8 field_0x146c8[0x178];
    u32 timer;  // 0x14840
    u8 pad_0x14844[0x7c];
    f32 delay;  // 0x148c0
};
static_assert(__builtin_offsetof(UnkNailongAIComponent_l6218f4, delay) == 0x148c0, "component layout");

// The object as a CNailongAIComponent, or null when it is not one.
static inline UnkNailongAI_l6218f4* AsNailong(UnkNailongAI_l6218f4* ai) {
    if (!ai)
        return nullptr;
    const CClass* cls = ai->vf_0x0();
    fn_710061f464();
    return fn_710009d50c(cls, &g_unk_71726f7e68) ? ai : nullptr;
}

// A random number in [0, 1].
static inline f32 Random01() { return (rand() & 0x7fff) * (1.0 / 32767.0); }

void fn_71006218f4(UnkNailongAIComponent_l6218f4* self, void* which) {
    fn_7101106490(reinterpret_cast<UnkShakernautAI_b64a4*>(self), which);
    UnkNailongAI_l6218f4* ai = AsNailong(self->owner->ai);
    bool isAttack = which == self->field_0x146c8;
    f32 min = isAttack ? ai->field_0x146e8->field_0xf0 : ai->field_0x14780->field_0xec;
    f32 max = isAttack ? ai->field_0x146e8->field_0xf4 : ai->field_0x14780->field_0xf0;
    f32 t = Random01();
    self->timer = 0;
    self->delay = min + (max - min) * t;
}
