// CAbility's unit (tools/tu_map.py): whether the ability's amount (virtual 0xc0) covers `cost`: true when
// the amount is zero within 0.001, else when the cost is above 1e-5 and at most the amount. Names are
// placeholders.
#include "types.h"

struct UnkAbility_A2_b72 {
    void** vtable;
    f32 vf_0xc0() { return reinterpret_cast<f32 (*)(UnkAbility_A2_b72*)>(vtable[0xc0 / 8])(this); }
};

bool fn_71002f93a8(UnkAbility_A2_b72* self, f32 cost) {
    f32 amount = self->vf_0xc0();
    if (amount + 0.001f >= 0.0f && amount + -0.001f <= 0.0f)
        return true;
    if (cost > 1e-5f)
        return self->vf_0xc0() <= cost;
    return false;
}
