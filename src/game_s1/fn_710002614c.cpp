// The matrix of a transform (UnkXform, include/game/UnkXformNode.h), rebuilt from its
// translation, rotation angles and scale (fn_71000791a8) when it is not valid; the angles and
// scale are derived from the old matrix first if they are not cached yet.
#include "base/UnkCloneHelpers.h"
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);

static inline void SetMatrix(UnkXform* x, const UnkMatrix44& r) {
    for (int i = 0; i < 16; ++i)
        reinterpret_cast<a_u32*>(x->m)[i] = reinterpret_cast<const a_u32*>(r.m)[i];
}

const UnkMatrix44& fn_710002614c(const UnkMatrix44* m) {
    UnkXform* x = reinterpret_cast<UnkXform*>(const_cast<UnkMatrix44*>(m));
    if (!(x->flags & 8)) {
        SetMatrix(x, fn_71000791a8(x->GetTranslation(), x->GetAngle(), x->GetScale()));
        x->flags |= 8;
    }
    return *m;
}
