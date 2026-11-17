// CChozoCommanderTriComboAttack's unit: scales the attack's displacement by how far ahead of the
// owner its target stands, while one of the combo's animations plays. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkTriComboAbilitySub {
    u8 pad_0x0[0x10];
    u32 state;
};

struct UnkTriComboAbility {
    u8 pad_0x0[0x20];
    UnkTriComboAbilitySub* sub;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    u32 state;  // 0xb4
    u8 pad_0xb8[0x438 - 0xb8];
    s32 field_0x438;
    u8 pad_0x43c[0x46c - 0x43c];
    f32 field_0x46c;
};

struct UnkTriComboAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x5c - 0x10];
    f32 field_0x5c;  // the displacement scale
};

struct UnkTriComboDisplacement {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
};

static inline f32 Sign(f32 v) {
    return static_cast<f32>((v > 0.0f) - (v < 0.0f));
}

static inline bool IsActive(UnkTriComboAbility* ability) {
    u32 state = ability->state;
    if (state == 7) {
        if (!ability->sub)
            return true;
        state = ability->sub->state;
    }
    return state >= 3;
}

// `from` below `start`, `to` from `start + range` on, linear between.
static inline f32 Ramp(f32 x, f32 start, f32 range, f32 from, f32 to) {
    f32 t = (x - start) / range;
    t = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    f32 below = (1.0f - t) * from;
    return t * to + below;
}

void fn_710043bdd4(UnkTriComboAttack* self, UnkTriComboDisplacement* displacement) {
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!animation || !target)
        return;

    f32 facing = Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2]);
    f32 ahead = (target->field_0x44 - self->owner->field_0x44) * facing;
    if (fn_710117ae38(animation, 0xf2de7855ef1809d2ull, false) ||
        fn_710117ae38(animation, 0x310a576b96c2109full, false)) {
        UnkTriComboAbility* ability = reinterpret_cast<UnkTriComboAbility*>(fn_7100df554c(g_unk_7101d47310));
        if (ability && fn_7100309568(reinterpret_cast<UnkAbility*>(ability)) - 3 < 4) {
            f32 side = Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38)).m[2]);
            if (ability->field_0x438 != 1)
                side = -side;
            f32 reach = IsActive(ability) ? ability->field_0x46c : 0.0f;
            f32 scale = Ramp(ahead + side * reach * facing, 800.0f, 500.0f, 0.5f, 2.0f);
            f32 current = self->field_0x5c;
            self->field_0x5c = current > scale ? current : scale;
        }
    } else if (fn_710117ae38(animation, 0x6369516fadbe7bd2ull, false)) {
        self->field_0x5c = Ramp(ahead, 100.0f, 400.0f, 0.5f, 1.4f);
    }

    if (self->field_0x5c > 0.0f &&
        fn_710117b348(animation, fn_7100d2f3c8())) {
        f32 scale = self->field_0x5c;
        scale = !(scale < 1.0f) ? scale : 1.0f;
        self->field_0x5c = scale;
        f32 x = displacement->field_0x60;
        f32 y = displacement->field_0x64;
        f32 z = displacement->field_0x68;
        displacement->field_0x6c = x * scale;
        displacement->field_0x70 = scale * y;
        displacement->field_0x74 = scale * z;
    }
}
