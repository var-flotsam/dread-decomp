// Multiplies the scale of a transform node (include/game/UnkXformNode.h) by `factor`, unless the
// factor is 1 within fn_7100217f60's tolerance, and marks the node dirty.
#include "game/UnkXformNode.h"

bool fn_7100217f60(f32 a, f32 b);  // whether a and b differ

void fn_710020aafc(UnkXformNode* self, f32 factor) {
    if (!fn_7100217f60(1.0f, factor))
        return;
    UnkXform* x = self->Xform();
    UnkVec3f s = x->GetScale();
    x->SetScale(UnkVec3f(s.x * factor, s.y * factor, s.z * factor));
    self->dirty_0xfa = 1;
}
