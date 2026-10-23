// Out-of-line angle and scale getters of three objects that hold an UnkXform at +0x40
// (include/game/UnkXformNode.h): each derives and caches the value on first use.
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
