// Makes the slot `out` refer to the weak handle of `self` (created on first use), releasing what it
// referred to before.
#include "game_s2/UnkWeakHandle.h"

void CAbilityOpticCamouflage_GetWeakHandle(UnkWeakTarget* self, UnkWeakHandle** out) {
    AssignRef(out, GetWeakHandle(self));
}
