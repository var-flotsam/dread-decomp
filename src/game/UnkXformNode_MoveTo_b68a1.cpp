// The transform node of include/game/UnkXformNode.h (unit 282): fn_7100206000 hands the node's target
// the node's position (vf_0x98) and `to` (fn_71001ba980, fn_71001ba864), marks the node and updates it
// once with no time (vf_0x20), as src/game/UnkXformNode.cpp's fn_7100206078 does with two updates.
// Names are placeholders.
#include "game/UnkTU244.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The node's virtual slots used here: its position (0x98) and an update taking a time step (0x20).
struct UnkXformNodeVtbl_b68a1 {
    u8 pad_0x0[0x20];
    void (*vf_0x20)(UnkXformNode* self, f32 dt);
    u8 pad_0x28[0x70];
    UnkVec3f (*vf_0x98)(UnkXformNode* self);
};

static inline UnkXformNodeVtbl_b68a1* Vtbl_b68a1(UnkXformNode* self) {
    return *reinterpret_cast<UnkXformNodeVtbl_b68a1**>(self);
}

// The target's previous and next positions (declared as in src/game/UnkXformNode.cpp).

void fn_7100206000(UnkXformNode* self, const UnkVec3f* to) {
    UnkXformTarget* target = self->target_0x108;
    UnkVec3f from = Vtbl_b68a1(self)->vf_0x98(self);
    fn_71001ba980(target, &from);
    fn_71001ba864(self->target_0x108, to);
    self->flag_0xfb = 1;
    Vtbl_b68a1(self)->vf_0x20(self, 0.0f);
}
