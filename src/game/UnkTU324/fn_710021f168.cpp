// Unit 324 (media instance): sets the state at +0x2c under the listener lock, keeping the old one at +0x30,
// and tells the listeners unless the new state is 1. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU324.h"
#include "game/media/UnkMedia324.h"
#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_710021f168(UnkMedia324* self, s32 state) {
    fn_710006691c(&self->listener_lock);
    bool notified = false;
    if (state >= 0) {
        self->previous_state = self->state;
        self->state = state;
        if (state != 1) {
            for (UnkMediaListener324* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
                it->callback(it->context, self, 0);
            notified = true;
        }
    }
    fn_7100066920(&self->listener_lock);
    return notified;
}
