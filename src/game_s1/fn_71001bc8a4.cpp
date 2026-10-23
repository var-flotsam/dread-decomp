// Sets the rotation angles of the UnkXform at +0x84 (include/game/UnkXformNode.h) when the
// object is active (+0x81), and marks it dirty (+0x82).
#include "game/UnkXformNode.h"

struct UnkXformHolder_0x84 {
    u8 pad_0x0[0x81];
    bool active_0x81;
    u8 dirty_0x82;
    u8 pad_0x83;
    UnkXform xform;
};

void fn_71001bc8a4(UnkXformHolder_0x84* self, const UnkVec3f* v) {
    if (self->active_0x81) {
        self->xform.SetAngle(*v);
        self->dirty_0x82 = 1;
    }
}
