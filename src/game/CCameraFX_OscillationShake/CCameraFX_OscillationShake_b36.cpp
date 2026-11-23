// CCameraFX_OscillationShake's reflected construction, as CCameraFX_RandShake's
// (src/game/CCameraFX_RandShake/fn_71010f2b68.cpp): the class's constructor, inlined (the base
// constructor, the virtual table, an empty sFunction and the reflected members zeroed), then the virtual
// call in slot 3.
#include <new>

#include "reflected/CCameraFX_OscillationShake.h"
#include "types.h"

void CCameraFX_OscillationShake::refl_Construct(void* obj) {
    CCameraFX_OscillationShake* self = new (obj) CCameraFX_OscillationShake();
    self->vf_0x18();
}
