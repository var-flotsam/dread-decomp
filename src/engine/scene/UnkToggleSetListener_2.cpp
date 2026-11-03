// More copies of fn_71001fb21c (setting an item's owner), one per unit (tools/tu_map.py units
// 272, 275, 278, 282 and their neighbours): the item reports its effective flags at 0xf8 and 0xf6,
// gated by the one at 0xf9, to the new owner. Names are placeholders.
#include "engine/scene/UnkToggle.h"

static inline void SetListener(UnkToggle* self, UnkToggleListener* listener) {
    self->listener_0x8 = listener;
    fn_71001eef58(listener, self, self->flag_0xf8 && self->flag_0xf9);
    fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
}

#define DEFINE_SET_LISTENER(name) \
    void name(UnkToggle* self, UnkToggleListener* listener) { SetListener(self, listener); }

DEFINE_SET_LISTENER(fn_71001f2bdc)
DEFINE_SET_LISTENER(fn_71001f5014)
DEFINE_SET_LISTENER(fn_71001fab38)
DEFINE_SET_LISTENER(fn_7100205df4)
DEFINE_SET_LISTENER(fn_7100206b3c)
DEFINE_SET_LISTENER(fn_7100207350)
DEFINE_SET_LISTENER(fn_7100208430)
DEFINE_SET_LISTENER(fn_710020a3c4)
DEFINE_SET_LISTENER(fn_710020b064)
DEFINE_SET_LISTENER(fn_710020d2b4)
