// CCameraFX_RandShake's reflected construction, as fn_71009b747c
// (src/game/CCharClassGrapplePointComponent/fn_71009b747c.cpp): the class's constructor, inlined (the
// base constructor, the virtual table, an empty sFunction and the other members zeroed), then the virtual
// call in slot 3.
#include <new>

#include "reflected/CCameraFX_RandShake.h"
#include "types.h"

void CCameraFX_RandShake::refl_Construct(void* obj) {
    CCameraFX_RandShake* self = new (obj) CCameraFX_RandShake();
    self->vf_0x18();
}
