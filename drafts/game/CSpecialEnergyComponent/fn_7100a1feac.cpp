// CSpecialEnergyComponent unit (unit 1535 of tools/tu_map.py): spends an amount of the energy at +0x44
// through fn_7100a1fa98 when there is some left and spending is not blocked (+0x48); the result says
// whether it did. With the energy at zero, +0x4c is cleared. Names and layouts are placeholders.
#include "game/CSpecialEnergyComponent.h"
#include "types.h"

struct UnkSpecialEnergy_a1feac {
    u8 pad_0x0[0x44];
    f32 energy;
    bool blocked;
    u8 pad_0x49[0x3];
    u32 field_0x4c;
};

bool fn_7100a1feac(UnkSpecialEnergy_a1feac* self, f32 amount) {
    f32 energy = self->energy;
    if (amount > 0.0f && !self->blocked) {
        if (energy > 0.0f) {
            fn_7100a1fa98(reinterpret_cast<UnkLifeComponent*>(self), false, true, energy - amount);
            return true;
        }
    }
    if (energy == 0.0f)
        self->field_0x4c = 0;
    return false;
}
