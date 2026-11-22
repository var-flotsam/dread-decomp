// CWarLotusAIComponent unit (tools/tu_map.py unit 1089): after the base update, refreshes the bump tuning
// from the parameters at +0x146c8 and, once the bump timer runs out, relaxes the impact factor towards zero
// and reports it as "BumpToRelaxToImpactFactor". Names and layouts are placeholders.
#include <math.h>
#include <float.h>
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CStrId;

struct UnkWarLotusBumpParams_l6e0868 {
    u8 pad_0x0[0x50c];
    f32 field_0x50c;
    f32 field_0x510;
    f32 field_0x514;
    f32 field_0x518;
};

struct UnkWarLotusBump_l6e0868 {
    UnkWarLotusBumpParams_l6e0868* params;  // 0x0
    f32 field_0x8;
    f32 field_0xc;
    f32 relax_rate;  // 0x10
    f32 field_0x14;
    f32 field_0x18;
    f32 timer;           // 0x1c
    f32 impact_factor;   // 0x20
    bool relaxing;       // 0x24
};

struct UnkWarLotusBagOwner_l6e0868 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;  // 0x80
};

struct UnkWarLotusOwner_l6e0868 {
    u8 pad_0x0[0x220];
    UnkWarLotusBagOwner_l6e0868* bag_owner;  // 0x220
};

struct UnkWarLotusAI_l6e0868 {
    u8 pad_0x0[0x30];
    UnkWarLotusOwner_l6e0868* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkWarLotusBump_l6e0868 bump;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkWarLotusAI_l6e0868, bump) == 0x146c8, "layout");

// Once the bump timer has run out: moves the impact factor (v) towards zero, reports it, and stops relaxing
// when it gets there.
static inline void Relax(UnkWarLotusAI_l6e0868* self, UnkWarLotusBump_l6e0868* bump, f32 v, f32 dt) {
    f32 step = bump->relax_rate * dt;
    f32 r;
    if (v < 0.0f) {
        r = v + step;
        r = r > 0.0f ? 0.0f : r;
    } else {
        r = v - step;
        r = r < 0.0f ? 0.0f : r;
    }
    bump->impact_factor = r;
    if (UnkWarLotusBagOwner_l6e0868* bag_owner = self->owner->bag_owner) {
        unk_prop_bag::UnkPropBag* bag = &bag_owner->bag;
        CStrId key("BumpToRelaxToImpactFactor", true);
        unk_prop_bag::fn_710033187c(bag, key, bump->impact_factor);
    }
    if (!(fabsf(bump->impact_factor) > FLT_EPSILON))
        bump->relaxing = false;
}

void fn_71006e0868(UnkWarLotusAI_l6e0868* self, f32 dt) {
    UnkWarLotusBump_l6e0868* bump = &self->bump;
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (UnkWarLotusBumpParams_l6e0868* params = bump->params) {
        bump->field_0x8 = 1.0f / params->field_0x50c;
        bump->field_0xc = 1.0f / params->field_0x510;
        bump->field_0x14 = params->field_0x514;
        bump->field_0x18 = params->field_0x518;
    }
    if (bump->timer > 0.0f || bump->relaxing) {
        bump->timer -= dt;
        if (bump->timer < 0.0f) {
            if (bump->relaxing) {
                Relax(self, bump, bump->impact_factor, dt);
            } else {
                f32 v = bump->impact_factor;
                if (fabsf(v) > FLT_EPSILON) {
                    bump->relaxing = true;
                    Relax(self, bump, v, dt);
                }
            }
        }
    }
}
