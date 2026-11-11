// CCharClassChozoZombieXAIComponent unit (tools/tu_map.py, the name from reflection): after the base
// character class's check (its answer unused), 2 when the actor's track is in the class fn_7100ce9338
// names, else 5 or 4 by bit 2 of the fourth argument's byte at +0xb6. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// As src/game/CCharClassSluggerAIComponent/CCharClassSluggerAIComponent_b38.cpp declares it (called with
// the same arguments).

struct UnkChozoZombieXInfo_b66a2 {
    u8 pad_0x0[0xb6];
    u8 flags;
};

u32 fn_710049f93c(void* self, UnkActor* actor, UnkActor* other, const UnkChozoZombieXInfo_b66a2* info) {
    fn_7100414160();
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    if (track && fn_710117b348(track, fn_7100ce9338())) {
        return 2;
    }
    return (info->flags & 4) ? 5 : 4;
}
