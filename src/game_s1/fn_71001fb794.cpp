// Sets the shown flag of a widget-like object; when it changes, tells the listener about the
// enabled and the effective-visible state (two notifier calls, the second one a tail call).
#include "types.h"

struct UnkListener;
struct UnkToggle {
    u8 pad_0x0[8];
    UnkListener* listener_0x8;
    u8 pad_0x10[0xe6];
    u8 flag_0xf6;
    u8 pad_0xf7;
    u8 flag_0xf8;
    u8 flag_0xf9;
};
void fn_71001eef58(UnkListener* l, UnkToggle* t, bool b);
void fn_71001eef74(UnkListener* l, UnkToggle* t, bool b);

void fn_71001fb794(UnkToggle* self, bool on) {
    if (on != self->flag_0xf9) {
        self->flag_0xf9 = on;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, self->flag_0xf8 && on);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
        }
    }
}
