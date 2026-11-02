// Two scene items (engine/scene/UnkToggle.h) of units 279 and 291 of tools/tu_map.py set their flag
// at 0xf8 the same way: when it changes, the owner is told (fn_71001eef58, gated by the flag at
// 0xf9), and the new value is copied into the object at +0x108 at +0x70 and at a per-unit offset
// (0x120 for fn_71001fb1b8, 0x35a for fn_710020dd40). Names are placeholders.
#include "engine/scene/UnkToggle.h"
#include "types.h"

template <u32 Offset>
static inline void SetShown(UnkToggle* self, bool on) {
    if (self->flag_0xf8 == on)
        return;
    self->flag_0xf8 = on;
    if (UnkToggleListener* owner = self->listener_0x8)
        fn_71001eef58(owner, self, on && self->flag_0xf9);
    char* target = reinterpret_cast<char*>(self->src_0x108);
    target[Offset] = on;
    target[0x70] = on;
}

void fn_71001fb1b8(UnkToggle* self, bool on) {
    SetShown<0x120>(self, on);
}

void fn_710020dd40(UnkToggle* self, bool on) {
    SetShown<0x35a>(self, on);
}
