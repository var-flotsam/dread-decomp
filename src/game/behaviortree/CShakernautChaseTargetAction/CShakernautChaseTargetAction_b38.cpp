// behaviortree::CShakernautChaseTargetAction: ending the action. After the base step, when the
// owner's AI component is a CShakernautAIComponent, clears its flag at +0x148a1 and hands its
// navigation object a default kind-5 parameter block. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkShakernautAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkShakernautAI*)>(vtable[0])(this);
    }
};

struct UnkShakernautChaseAction {
    u8 pad_0x0[0x18];
    UnkActor* owner;
};

void fn_71006ab8e4(UnkShakernautChaseAction* self) {
    fn_7100394bd4(self);
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    UnkShakernautAI* ai = static_cast<UnkShakernautAI*>(owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()))
        return;
    reinterpret_cast<u8*>(ai)[0x148a1] = 0;
    UnkNavigation* nav = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    if (!nav)
        return;
    UnkParams params;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    fn_71006f4b0c(nav, &params, 1);
}
