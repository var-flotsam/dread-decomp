// A state code from the actor's character class (its animation component at +0x220): 2 when it has a
// CCharClassAllowCrazyTrack (fn_710117b348), else 1. The same code is in CCharClassQuetzoaAIComponent's
// unit (972: fn_71006491cc) and CCharClassYamplotXAIComponent's unit (1108: fn_71006eeb40). Names are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

__attribute__((always_inline)) static inline u32 UnkCrazyTrackState_b49a3(UnkActor* actor) {
    auto* cls = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    if (!cls) {
        // Kept apart from the other 1 (the original returns it on its own path).
        u32 none = 1;
        asm("" : "+r"(none));
        return none;
    }
    return fn_710117b348(cls, fn_7100ce9338()) ? 2 : 1;
}

u32 fn_71006491cc(void* self, UnkActor* actor) { return UnkCrazyTrackState_b49a3(actor); }
u32 fn_71006eeb40(void* self, UnkActor* actor) { return UnkCrazyTrackState_b49a3(actor); }
