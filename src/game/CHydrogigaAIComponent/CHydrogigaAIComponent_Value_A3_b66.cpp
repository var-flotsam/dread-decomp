// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): fn_71005bf060 reports the value at
// +0x149d8 as "Value" in the property bag at +0x80 of the owner's +0x220 component (when there is
// one), then moves the value towards zero by twice `dt`, without crossing it. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkHydrogigaOwner_A3_b66 {
    u8 pad_0x0[0x220];
    char* field_0x220;
};

struct UnkHydrogigaAI_A3_b66 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_A3_b66* owner;
    u8 pad_0x38[0x149d8 - 0x38];
    f32 value;  // +0x149d8
};

void fn_71005bf060(UnkHydrogigaAI_A3_b66* self, f32 dt) {
    if (char* component = self->owner->field_0x220) {
        UnkPropBag* bag = reinterpret_cast<UnkPropBag*>(component + 0x80);
        CStrId key("Value", true);
        fn_710033187c(bag, key, self->value);
    }
    f32 step = dt + dt;
    f32 v = self->value;
    f32 r;
    if (v < 0.0f) {
        r = v + step;
        r = r > 0.0f ? 0.0f : r;
    } else {
        r = v - step;
        r = r < 0.0f ? 0.0f : r;
    }
    self->value = r;
}
