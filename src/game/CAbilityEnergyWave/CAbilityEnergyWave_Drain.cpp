// CAbilityEnergyWave's unit (tools/tu_map.py unit 429): through the part held at +0xe8, hands its
// +0x10 object the amount clamped to that object's +0x44 value, when the result is above
// FLT_EPSILON (fn_7100a1feac). Names are placeholders.
#include "types.h"

struct UnkTarget429 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

void fn_7100a1feac(UnkTarget429* self, f32 amount);

struct UnkPart429 {
    u8 pad_0x0[0x10];
    UnkTarget429* target;  // 0x10
};

struct UnkAbility429 {
    u8 pad_0x0[0xe8];
    UnkPart429* part;  // 0xe8
};

void fn_710030171c(UnkAbility429* self, f32 amount) {
    UnkPart429* part = self->part;
    if (part == nullptr) {
        return;
    }
    UnkTarget429* target = part->target;
    if (target == nullptr) {
        return;
    }
    f32 cap = target->field_0x44;
    f32 value = cap > amount ? amount : cap;
    if (value > 1.1920929e-07f) {
        fn_7100a1feac(target, value);
    }
}
