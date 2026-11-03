// In the CPullableGrapplePointComponent unit: adds (fn_7100d7c828) or removes (fn_7100d7c94c) one
// timeline id on the actor (+0x30). Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164ac40;
void fn_7100d7c94c(UnkActor* self, const u64* key);

struct UnkPullableGrapplePoint {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

void fn_710082cee4(UnkPullableGrapplePoint* self, bool add) {
    if (add)
        fn_7100d7c828(self->actor, &g_unk_710164ac40, false);
    else
        fn_7100d7c94c(self->actor, &g_unk_710164ac40);
}
