// Setting an item's owner, for items without a flag at 0xf8 (tools/tu_map.py units 273, 274, 276
// and 281, one copy each): the item reports its flag at 0xf9 as the first state, then the flag at
// 0xf6 gated by it, to the new owner (fn_71001eef58, fn_71001eef74). The copies read 0xf9 as a
// bool, without normalising it. Names are placeholders; see fn_71001fb21c for the full variant.
#include "engine/scene/UnkToggle.h"

static inline void SetListenerNoF8(UnkToggle* self, UnkToggleListener* listener) {
    self->listener_0x8 = listener;
    fn_71001eef58(listener, self, *reinterpret_cast<bool*>(&self->flag_0xf9));
    fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
}

#define DEFINE_SET_LISTENER_NO_F8(name) \
    void name(UnkToggle* self, UnkToggleListener* listener) { SetListenerNoF8(self, listener); }

DEFINE_SET_LISTENER_NO_F8(fn_71001f3750)
DEFINE_SET_LISTENER_NO_F8(fn_71001f4200)
DEFINE_SET_LISTENER_NO_F8(fn_71001f85e4)
DEFINE_SET_LISTENER_NO_F8(fn_710020c5d4)
