// CAbilityEnergyWave's unit (tools/tu_map.py unit 429): through the life component's handle at +0xe8, hands its
// +0x10 object the amount clamped to that object's +0x44 value, when the result is above
// FLT_EPSILON (fn_7100a1feac). Names are placeholders.
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkTarget429 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

void fn_7100a1feac(UnkTarget429* self, f32 amount);

void fn_710030171c(CAbilityComponent* self, f32 amount) {
    UnkWeakHandle* life = self->life;
    if (life == nullptr) {
        return;
    }
    UnkTarget429* target = reinterpret_cast<UnkTarget429*>(life->target);
    if (target == nullptr) {
        return;
    }
    f32 cap = target->field_0x44;
    f32 value = cap > amount ? amount : cap;
    if (value > 1.1920929e-07f) {
        fn_7100a1feac(target, value);
    }
}
