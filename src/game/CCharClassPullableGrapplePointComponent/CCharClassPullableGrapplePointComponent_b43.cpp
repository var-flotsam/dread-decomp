// CCharClassPullableGrapplePointComponent's unit (tools/tu_map.py): its constructor, on the grapple
// point base (fn_7100816b28): the vtable, two copies of the empty string id (g_unk_7101d470e0); the shape of fn_7100be0368 (src/game/CCharClassShowMessageEvent/CCharClassShowMessageEvent_b42.cpp).
// Names and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassGrapplePointComponent.h"
#include "game/CCharClassPullableGrapplePointComponent.h"
#include "types.h"

using base::global::CStrId;

// Signatures as src/game/CCharClassHangableGrappleSurfaceComponent/CCharClassHangableGrappleSurfaceComponent.cpp
// and src/game/CCharClassActionSwitcherOnPullGrapplePointComponent/CCharClassActionSwitcherOnPullGrapplePointComponent_b41.cpp
// declare them.
struct UnkSelf_as41;

struct UnkPullableGrapplePoint_b43a2 {
    void* vtable;
    u8 unk_0x8[0x60];
    CStrId id_0x68;
    CStrId id_0x70;
};

void fn_7100828940(UnkSelf_as41* raw) {
    UnkPullableGrapplePoint_b43a2* self = reinterpret_cast<UnkPullableGrapplePoint_b43a2*>(raw);
    fn_7100816b28(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a76348 + 0x10;
    new (&self->id_0x68) CStrId(g_unk_7101d470e0);
    new (&self->id_0x70) CStrId(g_unk_7101d470e0);
}
