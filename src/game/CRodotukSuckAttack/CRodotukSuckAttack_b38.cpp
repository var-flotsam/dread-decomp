// CRodotukSuckAttack: starts the attack when the owner's AI component is a CRodotukAIComponent,
// with two values read from the component (chosen by the flag at +0x68) and a third one.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkRodotukAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkRodotukAI*)>(vtable[0])(this);
    }
};

struct UnkSuckAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x58];
    bool field_0x68;
};

f32 fn_7100672328(UnkRodotukAI* ai);
f32 fn_7100672364(UnkRodotukAI* ai);
f32 fn_7100672418(UnkRodotukAI* ai);
bool fn_710066afd4(UnkSuckAttack* self, void* arg, f32 a, f32 b, f32 c, bool flag);

bool fn_710066aee8(UnkSuckAttack* self, void* arg) {
    UnkRodotukAI* ai = static_cast<UnkRodotukAI*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_7100671df8()))
        return false;
    f32 a = self->field_0x68 ? fn_7100672364(ai) : fn_7100672328(ai);
    f32 b = self->field_0x68 ? fn_7100672364(ai) - fn_7100672328(ai) : 0.0f;
    return fn_710066afd4(self, arg, a, b, fn_7100672418(ai), true);
}
