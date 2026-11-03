// CLifeComponent unit: taking and adding life by a fraction of the maximum (+0x50), as
// CLifeComponent_HealDamage.cpp's fn_71011601b8 and fn_7101160348 do by an amount. The life at +0x54 moves
// through vtable slot 0x168 with the new value and the old one (the maximum when healing); the result says whether it did. Names are
// placeholders.
#include "types.h"

struct UnkLifeValues_b49 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 max_life;  // 0x50
    f32 life;      // 0x54

    void vf_0x168(bool notify, f32 value, f32 old) {
        reinterpret_cast<void (*)(UnkLifeValues_b49*, bool, f32, f32)>(vtable[0x168 / 8])(this, notify, value, old);
    }
};

bool fn_7101160214(UnkLifeValues_b49* self, f32 fraction) {
    bool changed = false;
    f32 amount = self->max_life * fraction;
    if (amount > 0.0f) {
        f32 life = self->life;
        if (life > 0.0f) {
            self->vf_0x168(true, life - amount, life);
            changed = true;
        }
    }
    return changed;
}

bool fn_71011603a4(UnkLifeValues_b49* self, f32 fraction) {
    bool changed = false;
    f32 max_life = self->max_life;
    f32 amount = max_life * fraction;
    if (amount > 0.0f) {
        f32 life = self->life;
        if (life < max_life) {
            self->vf_0x168(true, life + amount, max_life);
            changed = true;
        }
    }
    return changed;
}
