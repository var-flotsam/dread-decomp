// Unit 282 (tools/tu_map.py): sets the translation of a transform node when it differs from the current one by more
// than 0.001 on any axis: stores it, marks the node dirty and hands its position (the node's vf_0x98) to the sphere at
// +0x80. Names and layouts are placeholders.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkXformNodeVtbl_b0249 {
    u8 pad_0x0[0x98];
    UnkVec3f (*vf_0x98)(UnkXformNode* self);
};

void fn_7100206194(UnkXformNode* self, const UnkVec3f* v) {
    UnkVec3f cur = self->Xform()->GetTranslation();
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        self->Xform()->SetTranslation(*v);
        self->dirty_0xfa = 1;
        UnkSphere* sphere = UnkXformNodeSphere(self);
        UnkVec3f pos = (*reinterpret_cast<UnkXformNodeVtbl_b0249**>(self))->vf_0x98(self);
        fn_7100216978(sphere, &pos);
    }
}
