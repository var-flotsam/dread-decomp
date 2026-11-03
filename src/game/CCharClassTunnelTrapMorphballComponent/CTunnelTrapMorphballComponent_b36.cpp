// Functions in the CCharClassTunnelTrapMorphballComponent unit (tools/tu_map.py; the
// CTunnelTrapMorphballComponent side). Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTunnelTrapMorphball {
    u8 pad_0x0[0x50];
    u8 field_0x50[0x18];        // the link registered with the target
    UnkWeakHandle* field_0x68;  // the weak handle of the target
};

void fn_7100a44074(UnkTunnelTrapMorphball* self);
void fn_7100a443c8(UnkTunnelTrapMorphball* self);

// Unregisters from the target (when the handle still has one) and drops the handle, then runs
// fn_7100a44074 or fn_7100a443c8 by `flag`.
void fn_7100a44350(UnkTunnelTrapMorphball* self, bool flag) {
    if (UnkWeakHandle* handle = self->field_0x68) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x50);
        ResetRef(&self->field_0x68);
    }
    if (flag)
        fn_7100a44074(self);
    else
        fn_7100a443c8(self);
}
