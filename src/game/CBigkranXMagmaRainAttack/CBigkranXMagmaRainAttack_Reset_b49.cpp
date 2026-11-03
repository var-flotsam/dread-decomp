// CBigkranXMagmaRainAttack (tools/tu_map.py): the reset. The shared attack reset (fn_71003609b8,
// game/UnkAttack.h), then the attack forgets its target (+0x30), unregisters its listener (+0x38) from
// the object behind the weak handle at +0x50 and drops the handle; finally, when the actor's AI
// component (+0x240) is a CBigkranXAIComponent (fn_71003f4454), clears its state (fn_71003f4f0c with 0).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkMagmaRainAI_b49 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkMagmaRainAI_b49*)>(vtable[0])(this); }
};
void fn_71003f4f0c(UnkMagmaRainAI_b49* ai, u32 state);

struct UnkMagmaRainActor_b49 {
    u8 pad_0x0[0x240];
    UnkMagmaRainAI_b49* ai;
};

struct UnkMagmaRainAttack_b49 {
    void** vtable;
    UnkMagmaRainActor_b49* actor;
    u8 pad_0x10[0x20];
    void* target;
    u8 link[0x18];
    UnkWeakHandle* handle;
};

void fn_71003ead78(UnkMagmaRainAttack_b49* self) {
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    self->target = nullptr;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target) {
            fn_710117b3f0(handle->target, self->link);
            if (!self->handle)
                goto done;
        }
        ResetRef(&self->handle);
    }
done:
    if (self->actor) {
        if (UnkMagmaRainAI_b49* ai = self->actor->ai) {
            if (fn_710009d50c(ai->vf_0x0(), fn_71003f4454()))
                fn_71003f4f0c(ai, 0);
        }
    }
}
