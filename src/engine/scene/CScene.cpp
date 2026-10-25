// Two notifiers of engine::scene::CScene's unit: an item whose state flag changed (the one at
// 0xf8 for fn_71001eef58, at 0xf6 for fn_71001eef74) reports it through its owner, which
// forwards the report to the delegate at its +8 as a tail call of one virtual for "on" and the
// next for "off"; the virtual receives the delegate and the item, not the flag.
#include "engine/scene/UnkToggle.h"

void fn_71001eef58(UnkToggleListener* owner, UnkToggle* item, bool on) {
    UnkToggleDelegate* d = owner->delegate_0x8;
    if (on)
        d->vtable_0x0->vf_0x40(d, item);
    else
        d->vtable_0x0->vf_0x48(d, item);
}

void fn_71001eef74(UnkToggleListener* owner, UnkToggle* item, bool on) {
    UnkToggleDelegate* d = owner->delegate_0x8;
    if (on)
        d->vtable_0x0->vf_0x50(d, item);
    else
        d->vtable_0x0->vf_0x58(d, item);
}
