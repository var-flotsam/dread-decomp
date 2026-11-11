// CCharClassPullableGrapplePointComponent's unit: the shape of fn_7100828940
// (CCharClassPullableGrapplePointComponent_b43.cpp) followed by the post-construction virtual call
// (slot 3): the grapple point base (fn_7100816b28), the vtable, two copies of the empty string id.
// Names and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassGrapplePointComponent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a76348[];

struct UnkPullableGrapplePoint_b53 {
    void** vtable;
    u8 unk_0x8[0x60];
    CStrId id_0x68;
    CStrId id_0x70;
};

void fn_7100828af4(UnkPullableGrapplePoint_b53* self) {
    fn_7100816b28(reinterpret_cast<char*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a76348 + 0x10);
    new (&self->id_0x68) CStrId(g_unk_7101d470e0);
    new (&self->id_0x70) CStrId(g_unk_7101d470e0);
    reinterpret_cast<void (*)(UnkPullableGrapplePoint_b53*)>(self->vtable[3])(self);
}
