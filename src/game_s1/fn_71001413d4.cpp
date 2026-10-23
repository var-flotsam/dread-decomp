// Objects that hold an UnkXform at +0x40 (include/game/UnkXformNode.h) and a transform node at
// +0x28. Out-of-line angle and scale getters of three of them (each derives and caches the value
// on first use), and two that apply a transform to the node while keeping their own scale
// (fn_7100140f8c, fn_7100145f80).
#include "game/UnkXformNode.h"

struct UnkXformHolder_0x40 {
    u8 pad_0x0[0x40];
    UnkXform xform;
};

UnkVec3f fn_71001413d4(UnkXformHolder_0x40* self) {
    return self->xform.GetAngle();
}

UnkVec3f fn_7100141444(UnkXformHolder_0x40* self) {
    return self->xform.GetScale();
}

UnkVec3f fn_7100146418(UnkXformHolder_0x40* self) {
    return self->xform.GetAngle();
}

UnkVec3f fn_7100146488(UnkXformHolder_0x40* self) {
    return self->xform.GetScale();
}

UnkVec3f fn_7100146f28(UnkXformHolder_0x40* self) {
    return self->xform.GetAngle();
}

UnkVec3f fn_7100146f98(UnkXformHolder_0x40* self) {
    return self->xform.GetScale();
}

void fn_71001f7220(char* node, const UnkXform* x);
void fn_7100207e9c(char* node, const UnkXform* x);

typedef void (*UnkXformApply)(char* node, const UnkXform* x);

template <UnkXformApply Apply>
static inline void ApplyKeepingScale(UnkXformHolder_0x40* self, const UnkXform* src) {
    UnkXform x(*src);
    UnkVec3f mine = self->xform.GetScale();
    char* node = reinterpret_cast<char*>(self) + 0x28;
    UnkVec3f s = x.GetScale();
    if (UNK_CHANGED(mine.x, s.x) || UNK_CHANGED(mine.y, s.y) || UNK_CHANGED(mine.z, s.z))
        x.SetScale(mine);
    Apply(node, &x);
}

void fn_7100140f8c(UnkXformHolder_0x40* self, const UnkXform* src) {
    ApplyKeepingScale<fn_71001f7220>(self, src);
}

void fn_7100145f80(UnkXformHolder_0x40* self, const UnkXform* src) {
    ApplyKeepingScale<fn_7100207e9c>(self, src);
}
