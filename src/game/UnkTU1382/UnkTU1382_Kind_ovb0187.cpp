// Unit 1382: a vector picked by kind. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f fn_71008e6254(const UnkVec3f* v, s32 kind) {
    if (u32(kind - 3) >= 2) {
        if (kind == 5 || kind == 2)
            return *reinterpret_cast<const UnkVec3f*>(&g_unk_710164779c);
        return *v;
    }
    return UnkVec3f(v->x, v->y, 0.0f);
}
