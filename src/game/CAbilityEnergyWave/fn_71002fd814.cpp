// Gives the holder (a handle slot) a weak reference when it has none or the one it has lost its target: the holder's
// handle is made to refer to the weak handle of the object fn_7100303054 finds in the owner (a
// refl_FindTyped instantiation, src/reflection_statics/), or to nothing when there is no such object.
#include "game/CAbilityEnergyWave.h"
#include "game_s2/UnkWeakHandle.h"

void fn_71002fd814(void* owner, UnkWeakHandle** holder) {
    if (!*holder || !(*holder)->target) {
        UnkWeakTarget* target = fn_7100303054(owner);
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(holder, handle);
    }
}
