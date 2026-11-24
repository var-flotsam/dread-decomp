// Makes the slot `out` refer to the weak handle of `self` (created on first use), releasing what it
// referred to before.
#include "game/CAbilityGhostAura.h"
#include "game_s2/UnkWeakHandle.h"

void CAbilityGhostAura_AssignWeakHandleToRef(UnkWeakTarget* self, UnkWeakRef* out) {
    AssignRef(&out->handle, GetWeakHandle(self));
}
