// Setting an item's owner (units 279 and 286 of tools/tu_map.py, one copy each): the item reports
// its effective flags at 0xf8 and 0xf6, gated by the one at 0xf9, to the new owner
// (fn_71001eef58, fn_71001eef74). Names are placeholders.
#include "engine/scene/UnkToggle.h"

static inline void SetListener(UnkToggle* self, UnkToggleListener* listener) {
    self->listener_0x8 = listener;
    fn_71001eef58(listener, self, self->flag_0xf8 && self->flag_0xf9);
    fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
}

void fn_71001fb21c(UnkToggle* self, UnkToggleListener* listener) { SetListener(self, listener); }
void fn_71002096d4(UnkToggle* self, UnkToggleListener* listener) { SetListener(self, listener); }
