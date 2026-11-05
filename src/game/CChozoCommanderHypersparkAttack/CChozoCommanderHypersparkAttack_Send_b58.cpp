// CChozoCommanderHypersparkAttack's unit (tools/tu_map.py): when the owner's (+0x8) component at +0x240 is a
// CChozoCommanderAIComponent, its sub-objects at +0x218, +0x220 and +0x298 and its actor (fn_7100d7c990)
// all exist and the stage at +0x58 is 3, sends the +0x220 sub-object a one (refl_EmitU32) and moves to
// stage 4. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkHypersparkSendOwner_b58 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    void* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
    u8 pad_0x248[0x50];
    void* field_0x298;
};

struct UnkHypersparkSend_b58 {
    u8 pad_0x0[0x8];
    UnkHypersparkSendOwner_b58* owner;
    u8 pad_0x10[0x48];
    s32 stage;
};

void fn_710042cb8c(UnkHypersparkSend_b58* self) {
    base::reflection::UnkReflObject* component = self->owner->field_0x240;
    bool isCommander = component ? fn_710009d50c(component->vf_0x0(), fn_710044e708()) : false;
    UnkHypersparkSendOwner_b58* owner = self->owner;
    void* target = owner->field_0x220;
    void* a = owner->field_0x218;
    void* b = owner->field_0x298;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
    if (isCommander && target && a && b && actor && self->stage == 3) {
        base::reflection::refl_EmitU32(target, 0x34fd472c06268b2e, 0xf4061ff25a2163d6, 1, &g_unk_710164b958,
                                       &g_unk_7172769740);
        self->stage = 4;
    }
}
