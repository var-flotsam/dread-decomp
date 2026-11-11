// CSpotLightComponent's unit (tools/tu_map.py): three reflected setters that forward a float or a bool
// to the light object behind the component (+0x58, its member at +0x20: fn_710020c9ac, fn_710020c740,
// fn_710020c494), as COmniLightComponent_Setters_ovb0033.cpp does. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU290.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkSpotLightObj_ovb0037 {
    u8 pad_0x0[0x20];
    u8 member[0x118];  // 0x20
};

struct UnkSpotLightComp_ovb0037 {
    u8 pad_0x0[0x58];
    UnkSpotLightObj_ovb0037* light;  // 0x58
};

UnkReflValue fn_71008f9d5c(void*, char* obj, UnkReflValue* const* args) {
    UnkSpotLightComp_ovb0037* self = *reinterpret_cast<UnkSpotLightComp_ovb0037**>(obj + 8);
    UnkReflValue* arg = args[0];
    f32 value = *base::reflection::refl_ArgValue<f32>(arg);
    fn_710020c9ac(self->light->member, value);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71008fa290(void*, char* obj, UnkReflValue* const* args) {
    UnkSpotLightComp_ovb0037* self = *reinterpret_cast<UnkSpotLightComp_ovb0037**>(obj + 8);
    UnkReflValue* arg = args[0];
    f32 value = *base::reflection::refl_ArgValue<f32>(arg);
    fn_710020c740(self->light->member, value);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71008fa34c(void*, char* obj, UnkReflValue* const* args) {
    UnkSpotLightComp_ovb0037* self = *reinterpret_cast<UnkSpotLightComp_ovb0037**>(obj + 8);
    UnkReflValue* arg = args[0];
    bool value = *base::reflection::refl_ArgValue<bool>(arg);
    fn_710020c494(self->light->member, value);
    return base::reflection::refl_EmptyValue();
}
