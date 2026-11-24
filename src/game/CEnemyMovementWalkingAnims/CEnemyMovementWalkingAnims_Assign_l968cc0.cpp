// CEnemyMovementWalkingAnims's unit (tools/tu_map.py, unit 1439): the copy-assignment of the walking
// animations' seven string ids, leaving the leading word alone. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkWalkingAnims_l968cc0 {
    void** vtable;
    CStrId sRelaxAnim;
    CStrId sFrontAnim;
    CStrId sBackAnim;
    CStrId sFrontInitAnim;
    CStrId sBackInitAnim;
    CStrId sFrontEndAnim;
    CStrId sBackEndAnim;
};

UnkWalkingAnims_l968cc0* fn_7100968cc0(UnkWalkingAnims_l968cc0* self, const UnkWalkingAnims_l968cc0* other) {
    self->sRelaxAnim = other->sRelaxAnim;
    self->sFrontAnim = other->sFrontAnim;
    self->sBackAnim = other->sBackAnim;
    self->sFrontInitAnim = other->sFrontInitAnim;
    self->sBackInitAnim = other->sBackInitAnim;
    self->sFrontEndAnim = other->sFrontEndAnim;
    self->sBackEndAnim = other->sBackEndAnim;
    return self;
}
