// Sets the shown flag of a widget-like object; when it changes, tells the listener about the
// enabled and the effective-visible state (two notifier calls, the second one a tail call).
#include "engine/scene/UnkToggle.h"

void fn_710020c128(UnkToggle* self, bool on) {
    if (on != self->flag_0xf9) {
        self->flag_0xf9 = on;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, self->flag_0xf8 && on);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
        }
    }
}
