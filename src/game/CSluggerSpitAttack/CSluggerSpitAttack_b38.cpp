// CSluggerSpitAttack: after the base attack's check, whether the spit can start: the target must
// exist and the state at +0x70 of the object reached through +0xe0 must be 2 (or that chain must be
// incomplete); then, when the owner's AI component is a CSluggerAIComponent, the attack's own check
// (fn_71006c1108) decides. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"
#include "game/CAttack.h"

struct UnkSluggerAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSluggerAI*)>(vtable[0])(this);
    }
};

struct UnkSpitState {
    u8 pad_0x0[0x70];
    u32 state;
};

struct UnkSpitLink2 {
    u8 pad_0x0[0x1b8];
    UnkSpitState* field_0x1b8;
};

struct UnkSpitLink {
    u8 pad_0x0[0x10];
    UnkSpitLink2* field_0x10;
};

struct UnkSpitAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0xd0];
    UnkSpitLink* field_0xe0;
};

// The base attack's check (declared elsewhere without its arguments; called with them).
struct UnkSelf;
bool fn_71006c1108(UnkSpitAttack* self);

bool fn_71006c1044(UnkSpitAttack* self, void* arg1, u32 arg2) {
    bool base = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg1, arg2);
    UnkActor* target = fn_7100d7c990(self->owner);
    bool ok = false;
    if (base && target) {
        UnkSpitLink* link = self->field_0xe0;
        if (link && link->field_0x10 && link->field_0x10->field_0x1b8)
            ok = link->field_0x10->field_0x1b8->state == 2;
        else
            ok = true;
    }
    UnkActor* owner = self->owner;
    UnkSluggerAI* ai = owner ? static_cast<UnkSluggerAI*>(owner->field_0x240) : nullptr;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71006c577c()) && ok)
        return fn_71006c1108(self);
    return ok;
}
