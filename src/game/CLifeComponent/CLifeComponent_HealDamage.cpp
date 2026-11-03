// CLifeComponent unit: adding and taking life. A positive amount moves the life at +0x54 (bounded
// above by the maximum at +0x50 for healing, needing some life left for damage) through vtable slot
// 0x168 with the new and the old value; the result says whether it did. Names are placeholders.
#include "types.h"

struct UnkLifeValues {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 max_life;  // 0x50
    f32 life;      // 0x54

    void vf_0x168(bool notify, f32 value, f32 old) {
        reinterpret_cast<void (*)(UnkLifeValues*, bool, f32, f32)>(vtable[0x168 / 8])(this, notify, value, old);
    }
};

bool fn_71011601b8(UnkLifeValues* self, f32 amount) {
    bool changed = false;
    if (amount > 0.0f) {
        f32 life = self->life;
        if (life > 0.0f) {
            self->vf_0x168(true, life - amount, life);
            changed = true;
        }
    }
    return changed;
}

bool fn_7101160348(UnkLifeValues* self, f32 amount) {
    bool changed = false;
    if (amount > 0.0f) {
        f32 life = self->life;
        if (life < self->max_life) {
            self->vf_0x168(true, life + amount, life);
            changed = true;
        }
    }
    return changed;
}
