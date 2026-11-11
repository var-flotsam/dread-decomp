// COmniLightComponent's unit (tools/tu_map.py): three reflected setters that forward to the light
// object behind the component (+0x58): a float and a bool to its member at +0x28 (fn_710020c9ac,
// fn_710020c494), and a float raised to at least 0.01 into its +0x140 -> +0x78 object (+0x110).
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU290.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkOmniLightData_ovb0033 {
    u8 pad_0x0[0x110];
    f32 field_0x110;
};
static_assert(__builtin_offsetof(UnkOmniLightData_ovb0033, field_0x110) == 0x110, "layout");

struct UnkOmniLightRender_ovb0033 {
    u8 pad_0x0[0x78];
    UnkOmniLightData_ovb0033* data;
};

struct UnkOmniLightObj_ovb0033 {
    u8 pad_0x0[0x28];
    u8 member[0x118];  // 0x28
    UnkOmniLightRender_ovb0033* render;  // 0x140
};

struct UnkOmniLightComp_ovb0033 {
    u8 pad_0x0[0x58];
    UnkOmniLightObj_ovb0033* light;
};

UnkReflValue fn_71008f2074(void*, char* obj, UnkReflValue* const* args) {
    UnkOmniLightComp_ovb0033* self = *reinterpret_cast<UnkOmniLightComp_ovb0033**>(obj + 8);
    UnkReflValue* arg = args[0];
    f32 value = *base::reflection::refl_ArgValue<f32>(arg);
    fn_710020c9ac(self->light->member, value);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71008f2280(void*, char* obj, UnkReflValue* const* args) {
    UnkOmniLightComp_ovb0033* self = *reinterpret_cast<UnkOmniLightComp_ovb0033**>(obj + 8);
    UnkReflValue* arg = args[0];
    bool value = *base::reflection::refl_ArgValue<bool>(arg);
    fn_710020c494(self->light->member, value);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71008f23e4(void*, char* obj, UnkReflValue* const* args) {
    UnkOmniLightComp_ovb0033* self = *reinterpret_cast<UnkOmniLightComp_ovb0033**>(obj + 8);
    f32 v = *base::reflection::refl_ArgValue<f32>(args[0]);
    self->light->render->data->field_0x110 = v < 0.01f ? 0.01f : v;
    return base::reflection::refl_EmptyValue();
}
