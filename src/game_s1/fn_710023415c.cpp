// Sets the rotation angles of the UnkXform at +0x38 (include/game/UnkXformNode.h) and, when
// asked, reports the change through the owner's virtual at 0x118 (with the code 2).
#include "game/UnkXformNode.h"

struct UnkXformOwner_0x38;

struct UnkXformChange {
    u32 kind;
};

struct UnkXformOwner_0x38Vtbl {
    u8 pad_0x0[0x118];
    void (*vf_0x118)(UnkXformOwner_0x38* self, UnkXform* x, UnkXformChange* change);
};

struct UnkXformOwner_0x38 {
    UnkXformOwner_0x38Vtbl* vtbl;
    u8 pad_0x8[0x30];
    UnkXform xform;
};

void fn_710023415c(UnkXformOwner_0x38* self, const UnkVec3f* v, bool notify) {
    self->xform.SetAngle(*v);
    if (notify) {
        UnkXformChange change;
        change.kind = 2;
        self->vtbl->vf_0x118(self, &self->xform, &change);
    }
}
