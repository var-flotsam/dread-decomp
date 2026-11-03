// CObsydomithonAttack: fn_710062a768 runs the attack base step (fn_71003609b8), clears +0x50, drops
// the weak handle at +0x48 (first handing its target the object at +0x30, fn_710117b3f0), resets
// the two floats at +0x58 to -1 and the flag at +0x61, then, when the actor's AI component (+0x240)
// is a CObsydomithonAIComponent, clears its word at +0x1479c. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkObsydomithonAttack : UnkAttackBase {
    u8 pad_0x20[0x10];
    u8 field_0x30[0x18];
    UnkWeakHandle* field_0x48;
    u64 field_0x50;
    f32 field_0x58;
    f32 field_0x5c;
    u8 pad_0x60;
    bool field_0x61;
};

static inline bool IsObsydomithonAI(UnkReflObject* ai) {
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710062c51c());
}

static inline char* AsObsydomithonAI(UnkReflObject* ai) {
    return reinterpret_cast<char*>(IsObsydomithonAI(ai) ? ai : nullptr);
}

void fn_710062a768(UnkObsydomithonAttack* self) {
    fn_71003609b8(self);
    self->field_0x50 = 0;
    if (UnkWeakHandle* handle = self->field_0x48) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x30);
        ResetRef(&self->field_0x48);
    }
    self->field_0x58 = -1.0f;
    self->field_0x5c = -1.0f;
    self->field_0x61 = false;
    if (IsObsydomithonAI(static_cast<UnkReflObject*>(self->field_0x8->field_0x240))) {
        char* ai = AsObsydomithonAI(static_cast<UnkReflObject*>(self->field_0x8->field_0x240));
        asm("" : "+r"(ai));
        *reinterpret_cast<u64*>(ai + 0x1479c) = 0;
    }
}
