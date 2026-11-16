// Unit 291: sets a scene item's (engine/scene/UnkToggle.h) enabled flag at 0xf9; when it changes, the
// listener hears both effective flags, and with 0xf6 set the value is copied into the object at +0x108
// (at +0x358 and +0x70). Names are placeholders.
#include "engine/scene/UnkToggle.h"
#include "types.h"

// As fn_71001f3224 does: a change of 0xf9 is reported to the listener with both effective flags.
static inline void SetEnabled(UnkToggle* self, bool on) {
    if (on != self->flag_0xf9) {
        self->flag_0xf9 = on;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, on && self->flag_0xf8);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
        }
    }
}

void fn_710020db44(char* item, bool on) {
    UnkToggle* self = reinterpret_cast<UnkToggle*>(item);
    SetEnabled(self, on);
    if (self->flag_0xf6) {
        char* target = reinterpret_cast<char*>(self->src_0x108);
        target[0x358] = target[0x70] = on;
    }
}
