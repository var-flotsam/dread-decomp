// CShelmitAIComponent's unit (tools/tu_map.py): starts the plasma ray. Unless the owner's (+0x30)
// character class (+0x220) already plays track 0x84b4b8ccfe2bc01d, it is sent that track with mode 1
// (as src/game_s2/UnkSpitSend.h); when the send succeeds the owner goes to the lazily created manager's
// +0x3f0 object (fn_71006bd4f0) and the attack at +0x146d0 starts (fn_71006b641c). Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

void fn_71006bd4f0(void* self, UnkActor* actor);
void fn_71006b641c(void* attack);

struct UnkShelmitAI;

struct UnkShelmitStartAI_b61 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146d0 - 0x38];
    u8 field_0x146d0[8];
};

static inline UnkManager_71726af468* GetManager_b61() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

void fn_71006baf90(UnkShelmitAI* ai) {
    UnkShelmitStartAI_b61* self = reinterpret_cast<UnkShelmitStartAI_b61*>(ai);
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!cls)
        return;
    const u64 track = 0x84b4b8ccfe2bc01d;
    if (fn_710117ae38(cls, track, false))
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(cls, track, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask))
        return;
    fn_71006bd4f0(reinterpret_cast<u8*>(GetManager_b61()) + 0x3f0, self->owner);
    fn_71006b641c(self->field_0x146d0);
}
