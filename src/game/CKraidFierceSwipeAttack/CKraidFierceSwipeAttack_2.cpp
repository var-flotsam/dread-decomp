// CKraidFierceSwipeAttack: fn_71005f4444 drops the weak handle at +0x50 (first handing its target
// the object at +0x38, fn_710117b3f0), runs fn_71005f2020, then sets the flag at +0x149e1 of the
// actor's AI component (+0x240) cast to CKraidAIComponent. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKraidFierceSwipe : UnkAttackBase {
    u8 pad_0x20[0x18];
    u8 field_0x38[0x18];
    UnkWeakHandle* field_0x50;
};

void fn_71005f2020(UnkKraidFierceSwipe* self);

void fn_71005f4444(UnkKraidFierceSwipe* self) {
    if (UnkWeakHandle* handle = self->field_0x50) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x38);
        ResetRef(&self->field_0x50);
    }
    fn_71005f2020(self);
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    char* kraid = reinterpret_cast<char*>(fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? ai : nullptr);
    kraid[0x149e1] = true;
}
