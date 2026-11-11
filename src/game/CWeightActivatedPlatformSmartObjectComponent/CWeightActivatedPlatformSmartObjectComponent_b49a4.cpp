// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py): while the component is enabled
// (+0x558), runs fn_7100a0a2dc when switched on and the object at +0x190 is not in state 6, or
// fn_7100a09cd4 when switched off and there is no object or it is in state 6. Names are placeholders.
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "types.h"

struct UnkWeightPlatformState_b49a4 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkWeightPlatform_b49a4 {
    u8 pad_0x0[0x190];
    UnkWeightPlatformState_b49a4* field_0x190;
    u8 pad_0x198[0x558 - 0x198];
    bool enabled;
};

void fn_7100a0a2dc(UnkWeightPlatform_b49a4* self);

bool fn_7100a0d480(UnkWeightPlatform_b49a4* self, bool on) {
    if (self->enabled) {
        UnkWeightPlatformState_b49a4* s = self->field_0x190;
        if (on) {
            if (s != nullptr && s->state != 6)
                fn_7100a0a2dc(self);
        } else {
            if (s == nullptr || s->state == 6)
                fn_7100a09cd4(self);
        }
    }
    return false;
}
