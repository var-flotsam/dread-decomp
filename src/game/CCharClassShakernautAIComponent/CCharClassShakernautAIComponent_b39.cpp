// CCharClassShakernautAIComponent unit (tools/tu_map.py): a state code. The base code from
// fn_710041b180 (src/game/CCharClassRobotAIComponent/CCharClassRobotAIComponent_State.cpp) becomes 2
// while the actor's character class (+0x220) runs a CCharClassAllowCrazyTrack. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassRobotAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// Declared as src/game/CCharClassRobotAIComponent/CCharClassRobotAIComponent_State.cpp defines it.
struct UnkActor621;

u32 fn_71006ace64(void* self, UnkActor* arg, UnkActor621* actor) {
    u32 state = fn_710041b180(self, arg, actor);
    if (UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(arg->field_0x220))
        return fn_710117b348(cls, fn_7100ce9338()) ? 2 : state;
    // Kept apart from the other return, as in the original.
    asm("" : "+r"(state));
    return state;
}
