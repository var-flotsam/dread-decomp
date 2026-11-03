// CHyperBeamGun unit (tools/tu_map.py, the name from reflection): once per arming (the flag at +0x18fa),
// unless the owner's character class (+0x220 of the actor at +0x18) has the
// CCharClassIgnoreHyperBeamTrack track, runs fn_7100843a50 with 1, sets the flag and clears +0x18a0.
// Always answers false. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

void fn_7100843a50(char* self, bool on);

bool fn_7100846aac(char* self) {
    u8* armed = reinterpret_cast<u8*>(self + 0x18fa);
    if (*armed)
        return false;
    UnkActor* actor = *reinterpret_cast<UnkActor**>(self + 0x18);
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    if (cls == nullptr || fn_710117b348(cls, fn_7100c2fe88()))
        return false;
    fn_7100843a50(self, true);
    *armed = 1;
    *reinterpret_cast<u32*>(self + 0x18a0) = 0;
    return false;
}
