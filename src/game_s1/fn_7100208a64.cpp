// Turns a transform node's link to its target object (fn_71001bc740 and its kin) on or off. On
// (when asked and the node accepts it, vf_0xa0) the target follows the node; off, it follows only
// if +0xf7 is clear. When following and the link was off, the target is first moved and turned
// to the node's translation and angles (fn_71001bc740, fn_71001bc8a4, fn_71001bc69c). The
// target's +0x70 records whether it follows; a change of state is stored at +0xf5 and its inverse
// at +0x136.
#include "game/UnkXformNode.h"

struct UnkXformHolder_0x84;
void fn_71001bc740(UnkXformHolder_0x84* self, const UnkVec3f* v);
void fn_71001bc8a4(UnkXformHolder_0x84* self, const UnkVec3f* v);
void fn_71001bc69c(UnkXformHolder_0x84* self);

struct UnkLinkedXformNode;

struct UnkLinkedXformNodeVtbl {
    u8 pad_0x0[0xa0];
    bool (*vf_0xa0)(UnkLinkedXformNode* self);
};

struct UnkLinkedXformNode : UnkXformNode {
    u8 pad_0x110[0x26];
    bool unlinked_0x136;

    UnkXformHolder_0x84* Target() { return reinterpret_cast<UnkXformHolder_0x84*>(target_0x108); }
};

void fn_7100208a64(UnkLinkedXformNode* self, bool on) {
    bool linked = on && (*reinterpret_cast<UnkLinkedXformNodeVtbl**>(self))->vf_0xa0(self);
    bool follow = linked || !self->field_0xf7;
    if (!self->field_0xf5 && follow) {
        {
            UnkXformHolder_0x84* target = self->Target();
            UnkVec3f t = self->Xform()->GetTranslation();
            fn_71001bc740(target, &t);
        }
        {
            UnkXformHolder_0x84* target = self->Target();
            UnkVec3f a = self->GetAngle();
            fn_71001bc8a4(target, &a);
        }
        fn_71001bc69c(self->Target());
    }
    reinterpret_cast<bool*>(self->target_0x108)[0x70] = follow;
    if (self->field_0xf5 != linked) {
        self->field_0xf5 = linked;
        self->unlinked_0x136 = !linked;
    }
}
