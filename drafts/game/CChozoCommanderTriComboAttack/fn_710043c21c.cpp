// CChozoCommanderTriComboAttack's unit (tools/tu_map.py): the attack's start. After the base start
// (fn_710042bb3c) it sets +0x58 and +0x5c to -1; with the owner's (+0x8) animation (+0x220): when one of two
// tracks is playing it sends, toward the actor the owner acts against, one of two ids with the current track
// time, otherwise a third id; then raises the CChozoCommanderAIComponent's flag at +0x15f00 and hooks the
// callback fn_710043ce18 (link at +0x60, handle at +0x78) up to the animation. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoCommanderGroundAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

void fn_710043ce18();

// The value descriptor of a send that carries a time.
struct UnkSendTime_l43c21c {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

struct UnkTriComboAI_l43c21c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x15ef8];
    bool field_0x15f00;
};
static_assert(__builtin_offsetof(UnkTriComboAI_l43c21c, field_0x15f00) == 0x15f00, "layout");

struct UnkTriComboLink_l43c21c {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkTriComboAttack_l43c21c {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x48];
    f32 field_0x58;
    f32 field_0x5c;
    UnkTriComboLink_l43c21c link;  // 0x60
    UnkWeakHandle* handle;         // 0x78
};

void fn_710043c21c(UnkTriComboAttack_l43c21c* self) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode1;
    UnkSendTime_l43c21c time;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    fn_710042bb3c(reinterpret_cast<UnkSelf*>(self));
    self->field_0x58 = -1.0f;
    self->field_0x5c = -1.0f;
    auto* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!animation)
        return;
    const u64 track = 0xf4061ff25a2163d6;
    if (fn_710117ae38(animation, 0x059bd73c0f154b64, false) || fn_710117ae38(animation, 0x7e85a61e95d46d67, false)) {
        if (UnkActor* target = fn_7100d7c990(self->owner)) {
            u64 id = !(target->field_0x44 >= self->owner->field_0x44) ? 0xd386589a7392c424 : 0xb347df0dbeaadadd;
            mode1.id = 1;
            time = {0, 0, fn_7101177ea0(animation, track)};
            mask1.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, id, track, &mode1, &g_unk_710164b958, &time, &mask1);
        }
    } else {
        mode2.id = 1;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, 0xf2de7855ef1809d2, track, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
    CStrId current = *fn_7101177ae8(animation, track);
    auto* ai = static_cast<UnkTriComboAI_l43c21c*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
        ai->field_0x15f00 = true;
    UnkTriComboLink_l43c21c* link = &self->link;
    link->context = self;
    link->callback = fn_710043ce18;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, link);
        ResetRef(&self->handle);
    }
    fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
