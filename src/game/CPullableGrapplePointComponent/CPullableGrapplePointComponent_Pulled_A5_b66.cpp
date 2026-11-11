// CPullableGrapplePointComponent's unit: gives the owner's weak target (fn_71002b3b84) one of two ids
// (fn_71009c0f1c) by a flag, and on the first also fn_71009c0974(1). Names are placeholders.
#include "game/CNavMeshItemComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkPullableGrapplePoint_A5_b66 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;
};

void fn_710082cf08(UnkPullableGrapplePoint_A5_b66* self, bool pulled) {
    void* target = fn_71002b3b84(self->owner);
    if (!target)
        return;
    if (pulled) {
        fn_71009c0f1c(target, 0xb15b31b835c17820);
        fn_71009c0974(target, true);
    } else {
        fn_71009c0f1c(target, 0x13ae275a1bcd6558);
    }
}
