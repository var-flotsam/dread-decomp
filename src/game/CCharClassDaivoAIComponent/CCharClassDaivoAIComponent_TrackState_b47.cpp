// CCharClassDaivoAIComponent unit (tools/tu_map.py unit 765): which of two tracks the actor's
// character class (its +0x220) plays: 2 for CCharClassAllowCrazyTrack, 7 for
// CCharClassRevertMeleeDamageTrack, else 1. The first parameter is unused. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"

u32 fn_71004de93c(void* self, UnkActor* actor) {
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    if (!cls)
        return 1;
    if (fn_710117b348(cls, fn_7100ce9338()))
        return 2;
    return fn_710117b348(cls, fn_7100c47fe8()) ? 7 : 1;
}
