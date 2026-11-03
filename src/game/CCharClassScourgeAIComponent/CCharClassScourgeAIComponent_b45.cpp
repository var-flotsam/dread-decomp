// CCharClassScourgeAIComponent's unit (tools/tu_map.py; the name from reflection): a state code, 2
// while the actor's character class (+0x220) runs a CCharClassAllowCrazyTrack, else 1 (the shape of
// fn_71006ace64 without a base code). Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

u32 fn_710069e88c(void* self, UnkActor* actor) {
    if (UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(actor->field_0x220))
        return fn_710117b348(cls, fn_7100ce9338()) ? 2 : 1;
    return 1;
}
